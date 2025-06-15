<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : README.md
 ! Doc Authors   : sanchuanhehe <wyihe5520@gmail.com>
 !               |  Aoran Zeng  <ccmywish@qq.com>
 ! Contributors  :   Nil Null   <nil@null.org>
 !               |
 ! Created On    : <2025-06-14>
 ! Last Modified : <2025-06-15>
 ! ---------------------------------------------------------- -->

# DEB package

本文档说明了 `chsrc` 项目的 DEB 包的相关情况

<br>

## 相关文件

- `.github/workflows/pkg-deb.yml` - CI 配置文件
- `CI.md` - CI 情况说明

<wbr>

- `debian/` - DEB 包构建配置目录
- `BUILD.md` - 如何手动构建
- `deb.makefile` - DEB 包构建 Makefile
- `deb-installation-test.sh` - DEB 包 **已正确安装** 测试脚本

<br>


## 安装

如果你是普通用户，你应该从 [GitHub Releases](https://github.com/RubyMetric/chsrc/releases) 下载合适的 DEB package，然后运行以下命令安装：

```bash
sudo dpkg -i chsrc_*.deb
sudo apt-get install -f  # Fix any dependency issues
```

如果你是高级用户，你可以自己阅读本目录下的 [./BUILD.md](./BUILD.md) 来自己构建 DEB 包并按照上述同样的方式安装。

<br>


## `debian/` 目录结构

```
debian/
├── changelog      # 版本更新日志
├── compat         # debhelper 兼容性版本
├── control        # 包控制信息和依赖
├── copyright      # 版权信息
├── postinst       # 安装后脚本
├── prerm          # 卸载前脚本
└── rules          # 构建规则
```

其中，最后三个是 `+x` 的可执行文件。

<br>
