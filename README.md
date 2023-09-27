<div align="center">
  <img alt="chsrc logo" src="chsrc.png"/>
</div>

全平台命令行换源工具，**目标支持 Linux, Windows, macOS, BSD 等尽可能多的操作系统，龙芯、飞腾等尽可能多的CPU**。 若有不支持的情况，请提交issue。

我们使用 **C99** 来完成上述目标。我们并不使用Python或JS等解释语言，因为一个简单的换源工具，不应该强行塞给用户一个庞大的解释器和数十、数百MB其他文件。

<br>

## 开发与维护

欢迎您参与维护你所熟悉的部分软件的换源。本软件为**自由软件**，**GPLv3** 许可证的采用将有机会使您的贡献被更广泛地使用，详情查阅 [LICENSE.txt](./LICENSE.txt)

参与维护开发，请查阅：[chsrc 协作者/维护者列表](https://gitee.com/RubyMetric/chsrc/issues/I7YI8E)

<br>

## 安装

使用 GitHub Actions 编译，请寻找下述与您的平台相匹配的命令来下载。

### Windows

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-x64-windows.exe -o chsrc.exe

# x86
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-x86-windows.exe -o chsrc.exe
```

### Linux

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-x64-linux -o chsrc; chmod +x ./chsrc

# aarch64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-aarch64-linux -o chsrc; chmod +x ./chsrc

# riscv64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-riscv64-linux -o chsrc; chmod +x ./chsrc

# armv7
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-armv7-linux -o chsrc; chmod +x ./chsrc
```

### macOS

```bash
# x64
curl -L https://gitee.com/RubyMetric/chsrc/releases/download/preview/chsrc-x64-macos -o chsrc; chmod +x ./chsrc
```

### 没有您的平台?

编译是一件很简单的事，不要担心，只需在命令行复制粘贴这一行运行:
```bash
git clone https://gitee.com/RubyMetric/chsrc.git ; cd chsrc ; make

# 运行试试看
./chsrc
```

<br>

## 使用

```bash
使用：chsrc <command> [target]

help                    # 打印此帮助，或 h, -h, --help
list (或 ls, 或 l)      # 列出可用镜像源，和可换源软件
list mirror/target      # 列出可用镜像源，或可换源软件
list os/lang/ware       # 列出可换源的操作系统/编程语言/软件
list <target>           # 查看该软件可以使用哪些源

cesu <target>           # 对该软件所有源测速
get  <target>           # 查看当前软件的源使用情况

set  <target>           # 换源，自动测速后挑选最快源
set  <target> def(ault) # 换源，默认使用维护团队测速第一的源
set  <target> <mirror>  # 换源，指定使用某镜像站
```

当你不想自动测速的时候，你可以直接指定某镜像站。

```bash
chsrc set ruby           # 测速，寻找最快者，换源
# 或
chsrc ls  ruby           # 列出可用的镜像站
chsrc set ruby rubychina # 使用 RubyChina 作为镜像站
```

注意: 下述语言/系统/软件均已实现基本支持，但还没有经过多平台测试，若有问题，请您及时提交报告。

部分换源方案当前实现不足，需要用户进行部分手动操作，您可对您熟悉的部分做出贡献。

<br>

## 编程语言开发

```bash
chsrc set ruby    或 set gem
chsrc set python  或 set pip
chsrc set node    或 set npm / nodejs / yarn # 同时会为yarn换源
chsrc set perl    或 set cpan
chsrc set php     或 set composer

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
sudo chsrc set debian
sudo chsrc set fedora
sudo chsrc set suse  或 set opensuse
sudo chsrc set arch  # 同时使用 archlinuxcn
sudo chsrc set manjaro
sudo chsrc set kali
sudo chsrc set gentoo
sudo chsrc set alpine
sudo chsrc set rocky
sudo chsrc set void

sudo chsrc set freebsd
sudo chsrc set openbsd
sudo chsrc set netbsd

# 国产操作系统
sudo chsrc set deepin
sudo chsrc set euler  或 set openeuler
sudo chsrc set kylin  或 set openkylin

chsrc set msys2 或 set msys
```

<br>

## 软件

```bash
chsrc set brew   或 set homebrew
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

make test  # 测试部分命令
make xy    # 测试 xy.h
make clean
```

<br>

## License

本软件采用 GPLv3 和 MIT 双许可证，具体请查阅 [LICENSE.txt](./LICENSE.txt)

<br>

## 致谢

感谢各个镜像站提供的优质免费镜像服务，使用的镜像站见 [sources.h](./sources.h).

特别感谢:
1. [MirrorZ 教育网镜像站](https://help.mirrors.cernet.edu.cn/)
2. [清华大学 Tuna](https://mirrors.tuna.tsinghua.edu.cn/)
3. [Thanks Mirror项目](https://github.com/eryajf/Thanks-Mirror) by [@eryajf](https://github.com/eryajf)

真诚鸣谢，各位参与的维护者

<br>
