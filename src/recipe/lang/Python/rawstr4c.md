<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 !               | happy game <happygame1024@gmail.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-12-17>
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