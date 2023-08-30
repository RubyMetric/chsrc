# chsrc

全平台命令行换源工具，**支持 Linux, Windows, macOS, BSD 等所有操作系统**。若有不支持的情况，请提交issue.

<br>

## 为什么不使用Python/Go/Rust?

1. `Python`: 许多人并非Python的拥趸，他们并不想在自己的系统上装满几十MB，乃至几百MB的Python文件。一个简单的换源工具，不需要也不应该强行塞给用户一个庞大的解释器和一堆文件

2. `Go`: Go还不足以编译到任何平台

3. `Rust`: 我不会Rust

<br>

## 安装使用

```bash
# 编程语言开发
chsrc gem   # 或 chsrc ruby
chsrc pip   # 或 chsrc python
chsrc npm   # 或 chsrc nodejs
chsrc cran  # 或 chsrc r
chsrc cpan  # 或 chsrc perl
chsrc julia

chsrc go
chsrc cargo # 或 chsrc rust 或 chsrc crate
chsrc maven
chsrc gradle
chsrc dotnet

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
chsrc anaconda
```

<br>

## 开发

请确保拥有一个支持C99的C编译器。

```bash
make
make test
make clean
```
