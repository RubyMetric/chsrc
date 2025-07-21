<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nil Null  <nil@null.org>
 ! Created On    : <2025-07-21>
 ! Last Modified : <2025-07-21>
 ! ---------------------------------------------------------- -->

# rawstr4c input for OS

- prefix = `RAWSTR_os`
- output = `:global-variable-only-header`
- translate = `:hex`

## openSUSE

- namespace = `openSUSE`

### Leap

`openSUSE Leap` 换源脚本

```bash
# 首先禁用官方软件源
zypper mr -da

zypper ar -cfg '@url@/distribution/leap/$releasever/repo/oss/' mirror-oss
zypper ar -cfg '@url@/distribution/leap/$releasever/repo/non-oss/' mirror-non-oss
zypper ar -cfg '@url@/update/leap/$releasever/oss/' mirror-update
zypper ar -cfg '@url@/update/leap/$releasever/non-oss/' mirror-update-non-oss

zypper ar -cfg '@url@/update/leap/$releasever/sle/' mirror-sle-update
zypper ar -cfg '@url@/update/leap/$releasever/backports/' mirror-backports-update
```

### Tumbleweed

`openSUSE Tumbleweed` 换源脚本

```bash
# 首先禁用官方软件源
zypper mr -da

zypper ar -cfg 'https://mirrors.tuna.tsinghua.edu.cn/opensuse/tumbleweed/repo/oss/' mirror-oss
zypper ar -cfg 'https://mirrors.tuna.tsinghua.edu.cn/opensuse/tumbleweed/repo/non-oss/' mirror-non-oss

# 刷新软件源
zypper ref
```
