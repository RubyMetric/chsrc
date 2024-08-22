<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2024-08-19>
 ! Last Modified : <2024-08-22>
 ! ---------------------------------------------------------- -->

# Write A Recipe Even If You Don't Know C

# 介绍

**`chsrc` 不仅是一个命令行工具，同时也是一个换源框架，它甚至使你能够在不了解C语言的情况下编写出新的换源方法(recipe)。**

我鼓励你为新的软件添加换源支持，因为通过 `chsrc` 这将非常简单，你的贡献也将非常有价值。

1. 本项目采用`GPLv3+`协议，是真正的**自由软件**，而非仅仅是开源软件
2. 代码规范灵活遵循`GNU`标准（若标准干扰了可维护性，则并不采纳）
3. 高度模块化，目录结构清晰易懂
4. 易于将`shell`脚本转换为等价的`recipe`
5. 已有大量`recipe`可提供参考
6. 提供了 [recipe template] 供直接使用

成功案例: [Armbian](../src/recipe/os/APT/Armbian.c)

<br>

# 贡献指导

若有任何问题，可在 [GitHub discussions](https://github.com/RubyMetric/chsrc/discussions) 中询问和讨论

<br>

# 基本概念

1. `target`: 所要换源的目标

2. `category`: 是 `target category` 的缩写，即 `target` 所属的类别，可以是 **编程语言**，**操作系统**，**软件** 三类之一

    1. 在目录中，三者分别为 `lang`, `os`, `ware`
    2. 在代码中，三者前缀分别为 `pl`, `os`, `wr`

2. `mirror`: 是 `mirror site` 的缩写，指镜像站，如清华大学开源软件镜像站
3. `source`: 该 `target` 所能换的具体的源，由 `mirror` 提供服务
4. `recipe`: 是为一个 `target` 定义的具体换源方法，请参考 `src` 目录中的 `recipe` 目录

5. `feature`: 一个 `target` 可以支持的功能，比如能否重置回默认上游源等

<br>

# 编写recipe步骤

1. 务必使用 `dev` 分支

2. 在 `recipe` 目录中根据类别添加新文件，该文件可基于其它 `recipe` 或 [recipe template]

3. 最好的参照物是 [ruby.c recipe](../src/recipe/lang/Ruby.c)

4. 在各大镜像站寻找可用源；可以额外补充镜像站

5. 可以使用 `chsrc.h` 中暴露出的任何 `chsrc_` 开头的函数或宏

6. 在 `menu.c` 中添加用户可以使用的 `target` 别名

7. 构建并运行 `chsrc set <target>` 测试，若无问题可提交 Pull Request

<br>

# 不要制造UFO

开发准则：
1. Convention over Configuration
2. [NO UFO 原则: 不要乱丢文件到$HOME等目录，尤其是使用各种隐晦的文件名](https://www.yuque.com/ccmywish/blog/no-ufo)

`chsrc` 主程序不提供配置文件，不提供数据文件，干净无污染。那么在实现 `recipe` 的时候，除了备份文件外，也不要污染用户环境。

<br>

[recipe template]: ../src/recipe/template.c
