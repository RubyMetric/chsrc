<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GPL-3.0-or-later
 ! -------------------------------------------------------------
 ! Config Type   : rawstr4c (Markdown)
 ! Config Authors: @ccmywish
 ! Contributors  : Nil Null <nil@null.org>
 ! Created On    : <2025-07-22>
 ! Last Modified : <2026-09-05>
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
   chsrc <command> [options] [dish] [mirror]

命令:
   help,  h                 打印此帮助，或 -h, --help
   issue, i                 查看相关issue

   list, ls, l              列出可用镜像站和可换源菜品
   list  mirror|dish        列出支持的: 镜像站/换源菜品
   list  os|lang|ware       列出支持的: 操作系统/编程语言/软件
   list   <dish>            查看该菜品可用源与支持功能

   measure, m, cesu <dish>  对该菜品所有源测速

   get, g <dish>            查看该菜品当前源的使用情况

   set, s <dish>            换源，自动测速后挑选最快源
   set    <dish>  first     换源，使用维护团队测速第一的源
   set    <dish> <mirror>   换源，指定使用某镜像站 (通过list <dish>查看)
   set    <dish>  <URL>     换源，用户自定义源URL
   reset  <dish>            重置，使用上游默认使用的源

   scan                     只读检测本机可换源项目与当前源
   plan                     生成换源方案（只读，不执行）
   setup                    扫描并执行换源；默认只处理用户级全自动项

选项:
   -dry                       Dry Run，模拟换源过程，命令仅打印并不运行
   -scope=project|user|system 仅对本项目换源 / 用户级换源 / 系统级换源 (通过ls <dish>查看)
   -ipv6                      使用IPv6测速
   -en(glish)                 使用英文输出
   -no-color                  无颜色输出

维护:
   chsrc 尊重贡献者，我们甚至拥有 贡献者(Contributor_t) 这个结构体，所有贡献者的
   信息直接进入运行时流动，永远不会仅作为注释被编译器抹去。

   chsrc 的代码是有趣的，"餐厅比喻" 贯穿了整个程序，chsrc 作为服务员(waiter)接受客人
   对某菜品(dish)的请求(get,set,ls)，后厨按需备菜(prepare)，主厨(chef)和调味师(saucier)
   依据菜谱(recipe)出餐。换源(source)，就是切换该菜品食材的供应商(provider/mirror)

   源代码地址: https://github.com/RubyMetric/chsrc
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
   chsrc <command> [options] [dish] [mirror]

COMMANDS:
   help,  h                 Print this help, or -h, --help
   issue, i                 See related issues

   list, ls, l              List available mirror sites and supported dishes
   list  mirror|dish        List supported:  mirror sites/supported dishes
   list  os|lang|ware       List supported: OSes/Programming Languages/Softwares
   list   <dish>            View available sources and supporting features for <dish>

   measure, m, cesu <dish>  Measure velocity of all sources of <dish>

   get, g <dish>            View the current source state for <dish>

   set, s <dish>            Change source, select the fastest source by automatic speed measurement
   set    <dish>  first     Change source, select the fastest source measured by the maintainers team
   set    <dish> <mirror>   Change source, specify a mirror site (Via `list <dish>`)
   set    <dish>  <URL>     Change source, using user-defined source URL
   reset  <dish>            Reset  source to the upstream's default

   scan                     Read-only detection of manageable sources on this machine
   plan                     Build a setup plan (read-only)
   setup                    Scan and apply; user-level automatic items only by default

OPTIONS:
   -dry                       Dry Run. Simulate the source changing process, command only prints, not run
   -scope=project|user|system Change source only for this project / user level / system level (Via `ls <dish>`)
   -ipv6                      Speed measurement using IPv6
   -en(glish)                 Output in English
   -no-color                  Output without color

MAINTAIN:
   chsrc respects contributors. We even have the "Contributor_t" struct. All the info of
   them flows into runtime, and will never be erased by the compiler merely as comments.

   chsrc is interesting. The "restaurant metaphor" runs through the entire program. chsrc
   acts as a "waiter", accepting users' requests (get, set, ls) for a certain "dish". The
   kitchen "prepare"s the ingredients as needed. The "chef" and the "saucier" follow the
   "recipe" to serve the dish. Changing "source"s means changing the "provider" of the
   ingredients for that dish.

   Source Code: https://github.com/RubyMetric/chsrc
```

<br>



## for `chsrc -v`

- name = `for__v_CHINESE`

```
chsrc @ver@

Copyright (C) 2023-2026 曾奥然, 郭恒
许可证 GPLv3+：GNU GPL 第 3 版或更高版本 <https://gnu.org/licenses/gpl.html>
这是自由软件：您可以自由修改和分发它。
在法律允许的最大范围内，本软件按'原样'提供，不作任何明示或暗示的保证。

由作者：曾奥然、郭恒，协作者：Mikachu2333、Happy Game 以及各位贡献者开发。(详见 chsrc-main.c, 或 `chsrc ls <dish>`)
```

<br>



## for `chsrc -v -en`

- name = `for__v_ENGLISH`

```
chsrc @ver@

Copyright (C) 2023-2026 Aoran Zeng, Heng Guo
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by authors: Aoran Zeng, Heng Guo, collaborators: Mikachu2333, Happy Game, and contributors. (See chsrc-main.c, or `chsrc ls <dish>`)
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
