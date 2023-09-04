# chsrc

全平台命令行换源工具，**目标支持 Linux, Windows, macOS, BSD 等所有操作系统，龙芯、申威、飞腾、兆芯、海光等所有CPU**。

若有不支持的情况，请提交issue。

我们使用 `C99` 来完成上述目标。另外，我们并不使用Python或JS等解释语言，因为一个简单的换源工具，不应该强行塞给用户一个庞大的解释器和数十、数百MB其他文件。

<br>

一个人无法精通各种软件，也没有机会使用多样的平台环境，因此我邀请您参与本项目，维护你所熟悉的语言/操作系统/软件的换源方式以及可用源。

请查阅：[chsrc 协作者/维护者列表](https://gitee.com/RubyMetric/chsrc/issues/I7YI8E)

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

## 编程语言开发
- [x] `chsrc set ruby`  或 `chsrc set gem`

- [x] `chsrc set python` 或 `chsrc set pip`

- [x] `chsrc set node`   或 `chsrc set npm` 或 `chsrc set nodejs`

- [x] `chsrc set perl` 或 `chsrc set cpan`
- [x] `chsrc set php`  或 `chsrc composer`

- [x] `chsrc set go`
- [ ] `chsrc set rust`  或 `chsrc set cargo` 或 `chsrc crate`
- [ ] `chsrc set java`  或 `chsrc set maven` 或 `chsrc set mvn` 或 `chsrc gradle`

- [x] `chsrc set r` 或 `chsrc set cran`  (同时会为 `bioconductor` 换源)
- [ ] `chsrc set julia`

## 操作系统
- [ ] `chsrc set ubuntu`
- [ ] `chsrc set debian`
- [ ] `chsrc set fedora`
- [ ] `chsrc set arch`
- [ ] `chsrc set gentoo`
- [ ] `chsrc set kali`
- [ ] `chsrc set manjaro`

- [ ] `chsrc set deepin`
- [ ] `chsrc set openkylin`
- [ ] `chsrc set openeuler`

- [ ] `chsrc set openbsd`
- [ ] `chsrc set netbsd`

- [ ] `chsrc set msys2` 或 `chsrc set msys`

## 软件

- [ ] `chsrc set brew`  或 `chsrc set homebrew`
- [ ] `chsrc set emacs`
- [ ] `chsrc set conda` 或 `chsrc set anaconda`
- [ ] `chsrc set ctan`  或 `chsrc set tex`


<br>

## 开发

请安装好 `gcc` 和 `make` 以及 `curl`

```bash
make
make test
make clean
```
