<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -----------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: @yayoinoyume
 ! Created On    : <2026-11-06>
 ! Last Modified : <2026-11-06>
 ! ---------------------------------------------------------- -->

# rawstr4c input for pacman

- prefix = `RAWSTR_os_pacman`
- output = `:global-variable-only-header`
- translate = `:hex`
- no-postfix = `true`

## Omarchy

### Get channel

只读取 `[omarchy]` 段内未注释的 Server 行，输出 `stable`、`rc`、`edge` 或 `missing`。

- name = `get_channel_cmd`

```sh
awk '
  /^\[omarchy\]/ { insec = 1; found = 1; next }
  /^\[/          { insec = 0 }
  insec && /^[[:space:]]*Server[[:space:]]*=/ {
    if ($0 ~ /\/edge\//)     channel = "edge";
    else if ($0 ~ /\/rc\//)  channel = "rc";
    else if (channel == "")   channel = "stable";
  }
  END {
    if (!found)             print "missing";
    else if (channel == "") print "stable";
    else                    print channel;
  }
' @f@
```

### Rewrite Omarchy section

只重写 `[omarchy]` 段：删除段内全部 Server 行，再写入目标行和可选的中国镜像兜底行。

- name = `rewrite_cmd`

```sh
awk -v target='@1@' -v fallback='@2@' '
  /^\[omarchy\]/ {
    print;
    print "Server = " target;
    if (fallback != "") print "Server = " fallback;
    insec = 1;
    next
  }
  /^\[/ { insec = 0 }
  insec && /^[[:space:]]*Server[[:space:]]*=/ { next }
  { print }
' @f@ > @tmp@ && mv @tmp@ @f@
```

### Validate Omarchy section

确认 `[omarchy]` 段首行是目标行；中国镜像还要求存在同通道兜底行。

- name = `validate_cmd`

```sh
awk -v target='@1@' -v fallback='@2@' '
  /^\[omarchy\]/ { insec = 1; next }
  /^\[/          { insec = 0 }
  insec && /^Server = / {
    if (!seen) { first = $0; seen = 1 }
    if (fallback != "" && $0 == "Server = " fallback) has_fallback = 1
  }
  END {
    if (!seen || first != "Server = " target) exit 1;
    if (fallback != "" && !has_fallback) exit 1;
    exit 0;
  }
' @f@
```
