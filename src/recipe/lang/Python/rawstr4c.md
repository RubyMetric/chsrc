<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 !               | happy game <happygame1024@gmail.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-07-26>
 ! ---------------------------------------------------------- -->

# rawstr4c input for Python

- prefix = `RAWSTR_pl_python`
- output = `:global-variable-only-header`
- translate = `:hex`
- keep-postfix = `false`

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
@sed@ '/^\[\[index\]\]$/,/^default = true$/{s|^url = ".*"$|url = "@url@"|}' @f@
```


### Final uv cmd

```sh
grep -q '^[[index]]$' @f@ && @ucmd@ || @acmd@
```

<br>



## Rye

- name = `rye_config`

```toml
[[sources]]
name = "@1@"
url  = "@2@"
```
