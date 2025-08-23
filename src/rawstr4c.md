<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: 曾奥然 <ccmywish@qq.com>
 ! Contributors  : Nil Null <nil@null.org>
 ! Created On    : <2025-07-22>
 ! Last Modified : <2025-08-22>
 ! ---------------------------------------------------------- -->

# [rawstr4c] input for chsrc

`chsrc` 使用的 C标准 (最低要求) 是 `gnu11` (`c11` 的超集)，这也就是说，我们项目是可以，而且是 **推荐** 同时混用 `R"()"` 和 `rawstr4c` 的

`LLVM` 对 `R"()"` 的支持是在 2024年07月 以后。但是在 GitHub Actions 中，所有出现的 `LLVM` 版本都太低了，
这使得我们被迫把已经写过的 `R"()"` 全部再转换为 `rawstr4c`.

用户端的编译器一般比较新，然而可能也没有新到如此的地步，通过使用 `rawstr4c` 我们也放宽了用户对编译器的要求。

我们预计等2~3年后，在项目中重新开始 `R"()"` 的写法

<br>

- prefix = `RAWSTR_chsrc`
- output = `:global-variable-only-header`
- translate = `:oct`
- no-postfix = `true`

<br>

## 中文帮助

- name = `USAGE_CHINESE`

```
名称:
   chsrc - Change Source - (GPLv3+)

版本:
   @ver@

使用:
   chsrc <command> [options] [target] [mirror]

命令:
   help,  h                   打印此帮助，或 -h, --help
   issue, i                   查看相关issue

   list, ls, l                列出可用镜像站和可换源目标
   list  mirror|target        列出支持的: 镜像站/换源目标
   list  os|lang|ware         列出支持的: 操作系统/编程语言/软件
   list   <target>            查看该目标可用源与支持功能

   measure, m, cesu <target>  对该目标所有源测速

   get, g <target>            查看该目标当前源的使用情况

   set, s <target>            换源，自动测速后挑选最快源
   set    <target>  first     换源，使用维护团队测速第一的源
   set    <target> <mirror>   换源，指定使用某镜像站 (通过list <target>查看)
   set    <target>  <URL>     换源，用户自定义源URL
   reset  <target>            重置，使用上游默认使用的源

选项:
   -dry                       Dry Run，模拟换源过程，命令仅打印并不运行
   -local                     仅对本项目而非全局换源 (通过ls <target>查看支持情况)
   -ipv6                      使用IPv6测速
   -en(glish)                 使用英文输出
   -no-color                  无颜色输出

维护:
   邀请您担任  Chef, 为用户把关您熟悉的 recipe

   源代码地址: https://github.com/RubyMetric/chsrc
   成为维护者: https://github.com/RubyMetric/chsrc/issues/275
```

<br>



## 英文帮助

- name = `USAGE_ENGLISH`

```
NAME:
   chsrc - Change Source - (GPLv3+)

VERSION:
   @ver@

USAGE:
   chsrc <command> [options] [target] [mirror]

COMMANDS:
   help,  h                   Print this help, or -h, --help
   issue, i                   See related issues

   list, ls, l                List available mirror sites and supported targets
   list  mirror|target        List supported:  mirror sites/supported targets
   list  os|lang|ware         List supported: OSes/Programming Languages/Softwares
   list   <target>            View available sources and supporting features for <target>

   measure, m, cesu <target>  Measure velocity of all sources of <target>

   get, g <target>            View the current source state for <target>

   set, s <target>            Change source, select the fastest source by automatic speed measurement
   set    <target>  first     Change source, select the fastest source measured by the maintainers team
   set    <target> <mirror>   Change source, specify a mirror site (Via `list <target>`)
   set    <target>  <URL>     Change source, using user-defined source URL
   reset  <target>            Reset  source to the upstream's default

OPTIONS:
   -dry                       Dry Run. Simulate the source changing process, command only prints, not run
   -local                     Change source only for this project rather than globally (Via `ls <target>`)
   -ipv6                      Speed measurement using IPv6
   -en(glish)                 Output in English
   -no-color                  Output without color

MAINTAIN:
   We invite you to become a Chef to ensure the quality of recipes you are familiar with for users:

   Source Code:               @url@
   Become a Maintainer:       https://github.com/RubyMetric/chsrc/issues/275
```

<br>



## for `chsrc -v`

- name = `for__v`

```
chsrc @ver@
Copyright (C) 2025 Aoran Zeng, Heng Guo
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Aoran Zeng, Heng Guo and contributors. (See chsrc-main.c)
```

<br>



## for `chsrc issue`

- name = `for_issue`

```
我们同时在 GitHub 和 Gitee 接受 issue 和 Bug 报告:

  - https://github.com/RubyMetric/chsrc/issues
  - https://gitee.com/RubyMetric/chsrc/issues


欢迎参与具体任务:

   Shell auto-completion 终端命令自动补全:

        https://github.com/RubyMetric/chsrc/issues/204

   搜集上游默认源地址，帮助进行 chsrc reset:

        https://github.com/RubyMetric/chsrc/issues/111

   搜集测速地址，进行精准测速:

        https://github.com/RubyMetric/chsrc/issues/205

   帮助没有预编译的平台编写 shell 脚本:

        https://github.com/RubyMetric/chsrc/issues/230


支持的通用镜像站:
  - https://github.com/RubyMetric/chsrc/wiki

```

<br>



## 最后告诉用户一些维护信息

- name = `op_epilogue`

```

   * 精准测速: 能真实反映你未来使用该资源时的速度，因为它直接测量你关注的那个资源。
   * 模糊测速: 仅代表该镜像站提供服务的一个可能速度。因而可能会出现测速数值较高，但实际使用体验不佳的现象。
当你遇到模糊测速时，请尽可能向我们提交准确的测速链接: chsrc issue
```

<br>



[rawstr4c]: https://github.com/RubyMetric/rawstr4c
