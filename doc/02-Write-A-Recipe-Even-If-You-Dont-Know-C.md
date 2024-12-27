<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 02-Write-A-Recipe-Even-If-You-Dont-Know-C.md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2024-08-19>
 ! Last Modified : <2024-12-27>
 ! ---------------------------------------------------------- -->

# Write A Recipe Even If You Don't Know C

# 介绍

**`chsrc` 不仅是一个命令行工具，同时也是一个换源框架，它甚至使你能够在不了解C语言的情况下编写出新的换源方法(recipe)。**

<br>

我鼓励你为新的软件添加换源支持，因为通过 `chsrc` 这将非常简单，你的贡献也将非常有价值。理论上每一个 `recipe` 都需要有专人长时间维护 ([招募](https://github.com/RubyMetric/chsrc/issues/130))。

1. 本项目采用 `GPLv3+` 协议，是真正的**自由软件**，而非仅仅是开源软件
2. 代码规范灵活遵循 `GNU` 标准（若标准干扰了可维护性，则并不采纳）
3. 高度模块化，目录结构清晰易懂
4. 极小依赖，极易构建，只需要 `GCC` 或 `Clang` 即可编译
5. 易于将 `shell` 脚本转换为等价的 `recipe`
6. 已有大量 `recipe` 可提供参考，并提供了 [recipe template] 供直接使用
7. 提供持续关注镜像站可用性的协作平台:

    1. https://github.com/RubyMetric/chsrc/wiki
    2. https://github.com/RubyMetric/chsrc/discussions

<br>

成功案例:

1. [Armbian](../src/recipe/os/APT/Armbian.c)
2. [uv](../src/recipe/lang/Python/uv.c)

<br>

# 基本概念

1. `target`: 所要换源的目标

2. `category`: 是 `target category` 的简写，即 `target` 所属的类别，可以是 **编程语言**，**操作系统**，**软件** 三类之一

    1. 在目录中，三者分别为 `lang`, `os`, `ware`
    2. 在代码中，三者前缀分别为 `pl`, `os`, `wr`

3. `mirror`: 是 `mirror site` 的简写，指镜像站，如清华大学开源软件镜像站
4. `source`: 该 `target` 所能换的具体的源，由 `mirror` 提供服务，往往一个 `mirror` 会提供许多 `source`
5. `recipe`: 是为一个 `target` 定义的具体换源方法，请参考 `src` 目录中的 `recipe` 目录

6. `feature`: 一个 `target` 可以支持的功能，比如能否重置回上游默认源等

7. **镜像源**: 为了方便，**偶尔**我们将直接称`mirror`和/或`source`为**镜像源**，这只是一种方便性的称呼，可以统称二者，也可以根据上下文指代二者之一

<br>

# 编写 `recipe` 步骤

1. 确定你要编写的 `target` 的标准名称，创建 `Target-Name.c` 文件

    大小写需严格按官方，若名称包含空格，需使用 `-` 代替空格

2. 根据类别将上述文件放在 `recipe/` 目录的某个子目录中

3. 复制 [recipe template] 的内容到上述文件中，并替换 `<...>` 占位符

4. 参考现有 `recipe` 的写法

    1. 看一眼就能上手的参照物是 [PHP recipe](../src/recipe/lang/PHP.c)
    2. 最好的参照物是 [Ruby recipe](../src/recipe/lang/Ruby.c)
    3. 组换源参照物是 [Python Group recipe](../src/recipe/lang/Python/Python.c)

5. 在 [Wiki] 中记录的镜像站中寻找可用源；可以额外补充镜像站

    并创建或更新对应 `target` 的镜像站可用状态 [Wiki] 页

6. 可以使用这些函数:

    1. `framework/core.c` 中以 `chsrc_` 开头的所有函数或宏
    2. `xy.h` 中以 `xy_` 开头的所有函数或宏

7. 在 `recipe/menu.c` 中添加用户可以使用的 `target` 别名

8. [编译、运行、测试 (how?)](./01-Develop.md)，若无问题可提交 Pull Request

<br>

# 开发准则

1. 代码高度可移植

2. Convention over Configuration

3. [NO UFO 原则: 不要乱丢文件到$HOME等目录，尤其是使用各种隐晦的文件名](https://www.yuque.com/ccmywish/blog/no-ufo)

    `chsrc` 主程序不提供配置文件，不提供数据文件，干净无污染。那么在实现 `recipe` 的时候，除了备份文件外，也不要污染用户环境。

<br>

[recipe template]: ../src/recipe/recipe-template.c
[Wiki]: https://github.com/RubyMetric/chsrc/wiki
