<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: @ccmywish
 !               | @happy-game
 !               | @MingriLingran
 ! Contributors  :  Nil Null <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-12-29>
 ! ---------------------------------------------------------- -->

# rawstr4c input for Python

- prefix = `RAWSTR_pl_python`
- output = `:global-variable-only-header`
- translate = `:hex`
- no-postfix = `true`

<br>



## uv

### uv config source content

```toml
[[index]]
url = "@url@"
default = true

```


### Get uv config

```sh
grep -A 2 'index' @f@ | sed -n 's/^url = "\(.*\)"/\1/p'
```


### Get uv config on Windows

为什么不用 `Select-String`，原因见: <https://github.com/RubyMetric/chsrc/pull/328#issuecomment-3695577870>

还有另外一个好处：如果 `[[index]]` 配置被写入了多次，可以这样检查出来

我们倾向于使用脚本而非命令，因为内容较长，便于用户检查到底执行了什么内容。

```powershell
$lines = Get-Content @f@;

for($index=0; $index -lt $lines.Count; $index++) {
  if($lines[$index] -match '^\[\[index\]\]$') {
    $lines[$index..($index+3)]
  }
}
```


### Set uv config

```sh
@sed@ '/^\[\[index\]\]$/,/^default = true$/{s|^url = ".*"$|url = "@url@"|;}' @f@
```

### Set uv config on Windows

```powershell
powershell -Command "$content = Get-Content '@f@'; $content = $content -replace '^url = \".*\"$', 'url = \"@url@\"'; $content | Set-Content '@f@'"
```


### Test uv if set source

```sh
grep -q '^\[\[index]]$' @f@
```

### Test uv if set source on Windows

```powershell
powershell -Command "if (Get-Content @f@ | Select-String '^\[\[index\]\]$') { exit 0 } else { exit 1 }"
```

<br>



## Rye

- name = `rye_config`

```toml
[[sources]]
name = "@1@"
url  = "@2@"
```
