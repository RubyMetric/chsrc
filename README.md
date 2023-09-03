# chsrc

全平台命令行换源工具，**支持 Linux, Windows, macOS, BSD 等所有操作系统，飞腾，龙芯等所有CPU**

若有不支持的情况，请提交issue

<br>

## 为什么不使用Python/JS/Go/Rust?

1. `Python`/`JS`:

    一个简单的换源工具，不需要也不应该强行塞给用户一个庞大的解释器和数十、数百MB其他文件。

2. `Go`: Go 不足以编译到任何平台

3. `Rust`: 我不会 Rust

<br>

## 安装使用
```bash
使用：chsrc <command> [target]

help                    # 打印此帮助，或 h, -h, --help
list (或 ls, 或 l)      # 查看可用镜像源，和可换源软件
list mirror(s)          # 查看可用镜像源
list target(s)          # 查看可换源软件
list <target>           # 查看该软件可以使用哪些源

cesu <target>           # 对该软件所有源测速
get  <target>           # 查看当前软件的源使用情况

set  <target>           # 换源，自动测速后挑选最快源
set  <target> def(ault) # 换源，默认挑选经维护者测速排序第一的源
set  <target> <mirror>  # 换源，指定使用某镜像站
```

换源：
```bash
# 编程语言开发
chsrc set ruby   # 或 chsrc set gem
chsrc set python # 或 chsrc set pip
chsrc set node   # 或 chsrc set npm 或 chsrc set nodejs
chsrc set perl   # 或 chsrc set cpan
chsrc set php    # 或 chsrc composer

chsrc set go
chsrc set rust   # 或 chsrc set cargo 或 chsrc crate
chsrc set java   # 或 chsrc set maven 或 chsrc set mvn 或 chsrc gradle

#chsrc set dotnet # 暂无换源需求

chsrc set r     # 或 chsrc set cran
chsrc set julia

# 操作系统
chsrc set ubuntu
chsrc set debian
chsrc set fedora
chsrc set arch
chsrc set gentoo
chsrc set kali
chsrc set manjaro

chsrc set deepin
chsrc set openkylin
chsrc set openeuler

chsrc set openbsd
chsrc set netbsd

chsrc set msys2 # 或 chsrc set msys

# 软件
chsrc set brew  # 或 chsrc set homebrew
chsrc set emacs
chsrc set conda # 或 chsrc set anaconda
chsrc set ctan  # 或 chsrc set tex
```

<br>

## 开发

请确保拥有一个支持C99的C编译器如 `gcc` 和 `make`

```bash
make
make test
make clean
```
