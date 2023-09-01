# chsrc

全平台命令行换源工具，**支持 Linux, Windows, macOS, BSD 等所有操作系统，飞腾，龙芯等所有CPU**

若有不支持的情况，请提交issue

<br>

## 为什么不使用Python/JS/Go/Rust?

1. `Python`/`JS`:

    我们只想换源。一个简单的换源工具，不需要也不应该强行塞给用户一个庞大的解释器和数十、数百MB其他文件。

2. `Go`: Go 不足以编译到任何平台

3. `Rust`: 我不会 Rust

<br>

## 安装使用
```bash
使用：chsrc <command> [target]
help                  # 打印此帮助，或 h, -h, --help
list (或 ls, 或 l)    # 查看可用镜像源，和可换源软件
list mirror(s)        # 查看可用镜像源
list target(s)        # 查看可换源软件
list <target>         # 查看该软件可以使用哪些源
cesu <target>         # 对该软件所有源测速
get  <target>         # 查看当前软件的源使用情况
set  <target>         # 换源，自动测速后挑选最快源
set  <target> -1      # 1,2,3的1。换源，不测速，挑选经维护者测速排序的第一源
set  <target> -v      # 换源，并打印换源所执行的具体操作
```

换源：
```bash
# 编程语言开发
chsrc gem   # 或 chsrc ruby
chsrc pip   # 或 chsrc python
chsrc npm   # 或 chsrc nodejs
chsrc cpan  # 或 chsrc perl

chsrc go
chsrc cargo # 或 chsrc rust 或 chsrc crate
chsrc maven # 或 chsrc java 或 chsrc gradle
chsrc dotnet
chsrc composer # 或 chsrc php

chsrc cran  # 或 chsrc r
chsrc julia

# 操作系统
chsrc ubuntu
chsrc debian
chsrc fedora
chsrc arch
chsrc gentoo
chsrc kali
chsrc manjaro

chsrc deepin
chsrc openkylin
chsrc openeuler

chsrc openbsd
chsrc netbsd

# Windows
chsrc msys2 # 或 chsrc msys

# macOS
chsrc brew  # 或 chsrc homebrew

# 其他软件
chsrc emacs
chsrc anaconda
chsrc ctan     # Tex
```

<br>

## 开发

请确保拥有一个支持C99的C编译器如 `gcc` 和 `make`

```bash
make
make test
make clean
```
