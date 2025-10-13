<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 !               | happy game <happygame1024@gmail.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-08-09>
 ! ---------------------------------------------------------- -->

# rawstr4c input for Python

- prefix = `RAWSTR_pl_python`
- output = `:global-variable-only-header`
- translate = `:hex`
- no-postfix = `true`

<br>



## uv

### uv config source content (`uv.toml`)

```toml
[pip]
index-url = "@url@"
```

### uv config source content (`pyproject.toml`)

```toml
[tool.uv.pip]
index-url = "@url@"
```

### uv config cpython source content (`uv.toml`)

```toml
python-install-mirror = "@url@"
```

### uv config cpython source content (`pyproject.toml`)

```toml
[tool.uv]
python-install-mirror = "@url@"
```

<br>

## Rye

- name = `rye_config`

```toml
[[sources]]
name = "@1@"
url  = "@2@"
```
