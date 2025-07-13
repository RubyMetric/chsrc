<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-14>
 ! Last Modified : <2025-07-14>
 ! ---------------------------------------------------------- -->

# rawstr4c input

- output = `:global-variable-only-header`
- translate = `:oct`

## Anaconda

- prefix = `RAWSTR_wr_anaconda`
- keep-postfix = `false`
- name = `condarc`

```yaml
channels:
  - defaults
show_channel_urls: true
default_channels:
  - @1@/anaconda/pkgs/main
  - @1@/anaconda/pkgs/r
  - @1@/anaconda/pkgs/msys2
custom_channels:
  conda-forge: @1@/anaconda/cloud
  pytorch: @1@/anaconda/cloud
```
