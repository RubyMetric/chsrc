<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 10-如何编写recipe.md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2024-08-19>
 ! Last Modified : <2025-08-11>
 ! ---------------------------------------------------------- -->

# Write A Recipe Even If You Don't Know C

## 介绍

**`chsrc` 不仅是一个命令行工具，同时也是一个体现了 Ruby on Rails 思想的 MVC 换源框架，它甚至使你能够在不了解C语言的情况下编写出新的换源方法(recipe)。**

<br>

我鼓励你为新的软件添加换源支持，因为通过 `chsrc` 框架，这将比写 shell 脚本更加轻松，你的贡献也将非常有价值。理论上每一个 `recipe` 都需要有专人长时间维护 ([招募](https://github.com/RubyMetric/chsrc/issues/130))。

- 本项目采用 `GPLv3+` 协议，是真正的**自由软件**，而非仅仅是开源软件
- 代码规范灵活遵循 `GNU` 标准（若标准干扰了可维护性，则并不采纳）
- 高度模块化，目录结构清晰易懂
- 极小依赖，极易构建，只需要 `GCC` 或 `Clang` 即可编译 (`make` 和 `just`可简化编译，但不是必需的)
- 易于将 `shell` 脚本转换为等价的 `recipe`
- 已有大量 `recipe` 可提供参考，并提供了 [recipe template] 供直接使用
- 本仓库外的子项目 [rawstr4c] 帮助你在C语言中维护复杂的字符串
- [chsrc-bootstrap] 帮助你在没有预编译 `chsrc` 的平台上 bootstrap 自己

<br>

成功案例:

- [Armbian](../src/recipe/os/APT/Armbian.c)
- [uv](../src/recipe/lang/Python/uv.c)

<br>

## 基本概念

- `target`: 所要换源的目标
- `target group`: 一个 `target` 包含了多个子 `target`，比如 `Python group` 包含了该语言的多个包管理器

- `category`: 是 `target category` 的简写，即 `target` 所属的类别，可以是 **编程语言**，**操作系统**，**软件** 三类之一

    1. 在目录中，三者分别为 `lang`, `os`, `ware`
    2. 在代码中，三者前缀分别为 `pl`, `os`, `wr`

- `mirror`: 是 `mirror site` 的简写，指镜像站，如清华大学开源软件镜像站
- `source`: 该 `target` 所能换的具体的源，由 `mirror` 提供服务，往往一个 `mirror` 会提供许多 `source`
- `recipe`: 是为一个 `target` 定义的具体换源方法，请参考 `src` 目录中的 `recipe` 目录

- **换源链接**: 指镜像站所提供的某一个具体的换源使用的URL
- **测速链接**: 用来测速的URL，与 "换源链接" 不同，可分为 **精准测速** 和 **模糊测速**

- **镜像源**: 为了方便，**偶尔**我们将直接称`mirror`和/或`source`为**镜像源**，这只是一种方便性的称呼，可以统称二者，也可以根据上下文指代二者之一

<br>

## 编写 `recipe` 步骤

1. 确定你要编写的 `target` 的标准名称，创建 `Target-Name.c` 文件

    大小写需严格按官方，若名称包含空格，需使用 `-` 代替空格

2. 根据类别将上述文件放在 `recipe/` 目录的某个子目录中

3. 复制 [recipe template] 的内容到上述文件中，并替换 `<...>` 占位符

4. 参考现有 `recipe` 的写法

    1. 看一眼就能上手的参照物是 [PHP recipe](../src/recipe/lang/PHP.c)
    2. 最好的参照物是 [Ruby recipe](../src/recipe/lang/Ruby/Ruby.c)
    3. 组换源参照物是 [Python group recipe](../src/recipe/lang/Python/Python.c)

5. 在 [Wiki] 中记录的镜像站中寻找可用源；可以额外补充镜像站

6. 使用 chef DSL 定义 `_prelude()` 函数

    该函数将填充 target 所有的必要信息，包括维护信息、换源信息

7. [设置/修改 "换源 URL" 和 "测速 URL" (how?)](./11-如何设置换源链接与测速链接.md)

8. 按需实现 `_setsrc()` `_getsrc()` `_resetsrc()`， 可以使用这些函数:

    1. `framework/core.c` 中以 `chsrc_` 开头的所有函数或宏
    2. `xy.h` 中以 `xy_` 开头的所有函数或宏
    3. `chec.c` `chef-helper.c` 中以 `chef_` 开头的所有函数或宏

    一个简单的方法是，在 VS Code 中按快捷键 `Ctrl-T` 搜索上述三种前缀

9. 在 `recipe/menu.c` 中登记此 target

10. [编译、运行、测试 (how?)](./01-开发与构建.md)，若无问题可提交 Pull Request

<br>

## 开发准则

1. 代码高度可移植

2. Convention over Configuration

3. [NO UFO 原则: 不要乱丢文件到$HOME等目录，尤其是使用各种隐晦的文件名](https://www.yuque.com/ccmywish/blog/no-ufo)

    `chsrc` 主程序不提供配置文件，不提供数据文件，干净无污染。那么在实现 `recipe` 的时候，除了备份文件外，也不要污染用户环境。

<br>

[rawstr4c]: https://github.com/RubyMetric/rawstr4c
[chsrc-bootstrap]: ../bootstrap/
[recipe template]: ../src/recipe/recipe-template.c
[Wiki]: https://github.com/RubyMetric/chsrc/wiki
