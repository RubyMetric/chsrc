<div align="center">
  <img alt="chsrc logo" src="image/chsrc.png"/>
</div>

全平台命令行换源工具，**目标支持 Linux (包括麒麟、openEuler、deepin等), Windows, macOS, BSD 等尽可能多的操作系统，龙芯、飞腾、RISC-V 等尽可能多的CPU**。

我们使用 **C99** 来完成上述目标。我们并不使用Python或JS等解释语言，因为一个简单的换源工具，不应该强行塞给用户一个庞大的解释器和数十、数百MB其他文件。

本软件为**自由软件**，采用 GPLv3 和 MIT 双许可证。

<br>

## 示例

<div align="center">
  <img alt="chsrc logo" src="image/example.png"/>
</div>

<br>

## 需要你的帮助

如果你想要通过 `scoop`，`brew`，`yay` 等系统包管理工具来安装和更新`chsrc`，请帮助我们达到下面的要求。

- [ ] 缺乏 `AUR` 维护者
- [ ] 缺乏 `homebrew` 维护者
- [ ] `scoop` 维护者
- [x] `scoop` 要求 GitHub star 数量超过 **500** 以及/或 **150**个fork
- [x] `homebrew` 要求 GitHub 仓库 **>=30 forks**, **>=30 watchers** or **>=75 stars**
- [ ] `scoop` 要求英文输出

    `chsrc`本意进行中文输出，但是我们将尽可能提供选项来进行英文输出。该选项同时有利于BSD用户

请访问 [chsrc on GitHub](https://github.com/RubyMetric/chsrc)

若您可提供维护，请访问 [issue#16 on GitHub](https://github.com/RubyMetric/chsrc/issues/16)

<br>

## 安装

以下方式均下载到当前目录，可直接通过 `./chsrc` 运行。

### Windows

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-x64-windows.exe -o chsrc.exe

# x86
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-x86-windows.exe -o chsrc.exe
```

### Linux

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-x64-linux -o chsrc; chmod +x ./chsrc

# aarch64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-aarch64-linux -o chsrc; chmod +x ./chsrc

# riscv64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-riscv64-linux -o chsrc; chmod +x ./chsrc

# armv7
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-armv7-linux -o chsrc; chmod +x ./chsrc
```

### macOS

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/latest/chsrc-x64-macos -o chsrc; chmod +x ./chsrc
```

### BSD

```bash
git clone https://gitee.com/RubyMetric/chsrc.git; cd chsrc
clang -Iinclude src/chsrc.c -o chsrc
```

### 其他平台

```bash
git clone https://gitee.com/RubyMetric/chsrc.git; cd chsrc; make
```

<br>

## 使用

```bash
使用: chsrc <command> [options] [target] [mirror]

help                    # 打印此帮助，或 h, -h, --help
issue                   # 查看相关issue
list (或 ls, 或 l)      # 列出可用镜像源，和可换源软件
list mirror/target      # 列出可用镜像源，或可换源软件
list os/lang/ware       # 列出可换源的操作系统/编程语言/软件
list <target>           # 查看该软件可以使用哪些源

cesu <target>           # 对该软件所有源测速
get  <target>           # 查看当前软件的源使用情况

set  <target>           # 换源，自动测速后挑选最快源
set  <target>  first    # 换源，使用维护团队测速第一的源
set  <target> <mirror>  # 换源，指定使用某镜像站 (通过list命令查看)
reset <target>          # 重置，使用上游默认使用的源

选项:
-ipv6                   # 使用IPv6测速
-local                  # 仅对某项目而非全局换源 (仅部分软件如bundler,pdm支持)
```

当你**不想自动测速的时候**，你可以直接指定某镜像站，以及指定维护团队已测试的最快镜像站。

```bash
chsrc set ruby           # 测速，寻找最快者，换源
# 或
chsrc ls  ruby           # 列出可用的镜像站
chsrc set ruby rubychina # 使用 RubyChina 作为镜像站
# 或
chsrc set ruby first     # 使用维护团队测试的最快镜像站
```

对部分[支持局部换源](https://gitee.com/RubyMetric/chsrc/issues/I9V5I0)的，可以避免全局换源。
```bash
chsrc set -local bundler
chsrc set -local pdm
```

<br>

## 编程语言开发

```bash
chsrc set ruby    或 set gem
chsrc set python  或 set pip / pdm # 同时换pip和pdm
chsrc set node    或 set npm / nodejs / yarn / pnpm # 同时换3个
chsrc set perl    或 set cpan
chsrc set php     或 set composer
chsrc set lua     或 set luarocks

chsrc set go
chsrc set rust    或 set cargo / crate
chsrc set java    或 set maven / mvn / gradle
chsrc set clojure 或 set clojars
chsrc set dart    或 set pub / flutter # 同时会为flutter换源
chsrc set haskell 或 set hackage/cabal/stack
chsrc set ocaml   或 set opam

# 同时会为 bioconductor 换源
chsrc set r       或 set cran
chsrc set julia
```

<br>

## 操作系统

```bash
sudo chsrc set ubuntu
sudo chsrc set mint  或 linuxmint
sudo chsrc set debian
sudo chsrc set fedora
sudo chsrc set suse  或 set opensuse
sudo chsrc set kali
sudo chsrc set arch  # 同时使用 archlinuxcn
sudo chsrc set manjaro
sudo chsrc set gentoo
sudo chsrc set rocky 或 set rockylinux
sudo chsrc set alpine
sudo chsrc set void  或 set voidlinux
sudo chsrc set solus
sudo chsrc set ros   或 set ros2
sudo chsrc set trisquel
sudo chsrc set lite  或 set linuxlite
sudo chsrc set raspi 或 set raspberrypi

sudo chsrc set deepin
sudo chsrc set euler  或 set openeuler
sudo chsrc set kylin  或 set openkylin

chsrc set msys2 或 set msys

# BSD
sudo chsrc set freebsd
sudo chsrc set openbsd
sudo chsrc set netbsd
```

<br>

## 软件

```bash
chsrc set winget
chsrc set brew      或 set homebrew
chsrc set cocoapods 或 set cocoa / pod
chsrc set dockerhub 或 set docker
chsrc set flathub
chsrc set nix
chsrc set guix
chsrc set emacs  或 set elpa
chsrc set tex    或 set ctan / latex / texlive / miktex
chsrc set conda  或 set anaconda
```

<br>

## 开发

请安装好 `gcc`或`clang` 和 `make` 以及 `curl`

```bash
# 使用 dev 分支开发
git clone https://gitee.com/RubyMetric/chsrc.git -b dev

make          # 默认使用 cc 编译
make CC=clang # 使用 clang 编译
make CC=gcc   # 使用 gcc   编译

make test    # 测试命令
make test-xy # 测试 xy.h
make clean
```

<br>

## 致谢

感谢各个镜像站提供的优质免费镜像服务，使用的镜像站见 [source.h](./include/source.h).

另外感谢以下项目:
1. [MirrorZ 教育网镜像站](https://help.mirrors.cernet.edu.cn/)
2. [清华大学 Tuna](https://mirrors.tuna.tsinghua.edu.cn/)
3. [Thanks Mirror项目](https://github.com/eryajf/Thanks-Mirror) by [@eryajf](https://github.com/eryajf)

<br>
