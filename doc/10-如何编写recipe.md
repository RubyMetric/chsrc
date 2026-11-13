<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : 10-如何编写recipe.md
 ! Doc Authors   : @ccmywish
 ! Contributors  : @Mikachu2333
 !               |
 ! Created On    : <2024-08-19>
 ! Last Modified : <2026-10-28>
 ! ---------------------------------------------------------- -->

# Write A Recipe Even If You Don't Know C

## 介绍

**`chsrc` 不仅是一个命令行工具，同时也是一个体现了 Ruby on Rails 思想的 MVC 换源框架，它甚至使你能够在不了解C语言的情况下编写出新的换源方法(recipe)。**

<br>

我鼓励你为新的软件添加换源支持，因为通过 `chsrc` 框架，这将比写 shell 脚本更加轻松，你的贡献也将非常有价值。理论上每一个 `recipe` 都需要有专人长时间维护 ([招募](https://github.com/RubyMetric/chsrc/issues/275))。

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

- `dish`: 所要换源的菜品
- `combo dish`: 一个 `dish` 包含了多个子 `dish`，比如 `Python` 包含了该语言的多个包管理器

- `category`: 是 `dish category` 的简写，即 `dish` 所属的类别，可以是 **编程语言**，**操作系统**，**软件** 三类之一

    1. 在目录中，三者分别为 `lang`, `os`, `ware`
    2. 在代码中，三者前缀分别为 `pl`, `os`, `wr`

- `mirror`: 是 `mirror site` 的简写，指镜像站，如清华大学开源软件镜像站
- `source`: 该 `dish` 所能换的具体的源，由 `mirror` 提供服务，往往一个 `mirror` 会提供许多 `source`
- `recipe`: 是为一个 `dish` 定义的具体换源方法，请参考 `src` 目录中的 `recipe` 目录

- `chef DSL`：是 `chef Domain Specific Language` 的简写，这是一组以 `chef_` 开头的函数，用来定义维护者信息、可用源等元信息

- **换源链接**: 指镜像站所提供的某一个具体的换源使用的URL
- **测速链接**: 用来测速的URL，与 "换源链接" 不同，可分为 **精准测速** 和 **模糊测速**

    1. 在代码中，测速链接一般使用 `smURL` (即 `speed measure URL`) 或 `speed_measure_url` 来指代
    2. 在代码中，换源链接一般使用 `repoURL` (即 `repository URL`) 或直接用 `url` 来指代

        - 为什么不用 `regurl`，因为使用术语 `repository` 的 dish 远多于使用术语 `registry` 的。
        - 为什么莫名其妙使用了大小写混合的API? 因为 `smurl` 和 `repourl` 的可读性太差

- **镜像源**: 为了方便，**偶尔**我们将直接称`mirror`和/或`source`为**镜像源**，这只是一种方便性的称呼，可以统称二者，也可以根据上下文指代二者之一

<br>

## 编写 `recipe` 步骤

1. 确定你要编写的 `dish` 的标准名称，创建 `Dish-Name.c` 文件

    大小写需严格按官方，若名称包含空格，需使用 `-` 代替空格

2. 根据类别将上述文件放在 `recipe/` 目录的某个子目录中

3. 复制 [recipe template] 的内容到上述文件中，并替换 `<...>` 占位符

4. 参考现有 `recipe` 的写法

    1. 看一眼就能上手的参照物是 [PHP recipe](../src/recipe/lang/PHP.c)
    2. 最好的参照物是 [Ruby recipe](../src/recipe/lang/Ruby/Ruby.c)
    3. 组换源参照物是 [JS combo recipe](../src/recipe/lang/JavaScript/JavaScript.c)
    4. 环境变量型换源参照物是单变量的 [Pub recipe](../src/recipe/lang/Dart/Pub.c) 与多变量的 [rustup recipe](../src/recipe/lang/Rust/rustup.c)

5. 在 [Wiki] 中记录的镜像站中寻找可用源；可以额外补充镜像站

6. 使用 chef DSL 定义 `_prepare()` 函数

    该函数将填充 `dish` 所有的必要信息，包括维护信息、换源信息

7. [设置/修改 "换源链接" 和 "测速链接" (how?)](./11-如何设置换源链接与测速链接.md)

8. 按需实现 `_setsrc()` `_getsrc()` `_resetsrc()`， 可以使用这些函数:

    1. `framework/*.c` 中以 `chsrc_`, `chef_`, `hp_` 开头的所有函数或宏
    2. `xy.h` 中以 `xy_` 开头的所有函数或宏
    3. 若菜品通过**环境变量**持久化换源，请统一使用 `framework/env.c` 提供的跨平台幂等接口，**切勿**手工向 `.bashrc`/`.zshrc` 追加字符串或直接调用 Windows `setx`：
        - `chsrc_set_env (name, value)`：写入或更新单个持久化环境变量（传入 `NULL` 表示删除该变量，常用于 `_resetsrc()`）
        - `chsrc_set_env_block (name1, value1, ..., NULL)`：批量写入或删除多个环境变量。POSIX 先暂存全部内容，再逐文件原子替换；后续失败会回滚已替换文件。Windows 使用注册表快照回滚，操作结束统一提示/广播。多文件更新不保证进程崩溃或断电时的整体原子性。
        - `chsrc_get_env (var1, ..., NULL)`：读取并打印已持久化的环境变量值（持久化缺失时回退当前终端值；两者同时存在且不一致时会提示）
        - 作用域声明上，若某作用域官方支持但 `chsrc` 仅输出手工配置指引，可标记为 `ScopeCap_Able_But_Just_Docs`

    4. 普通配置文件可复用 `framework/file-update.c`：在内存中完成格式解析与内容转换后，
       使用 `chsrc_update_files()` 批量提交。当前提交后端支持 POSIX，接口、限制和配置归属策略见
       [12-配置文件更新.md](./12-配置文件更新.md)。文件层不负责识别镜像、区块或 JSON 字段。

    一个简单的方法是，在 VS Code 中按快捷键 `Ctrl-T` 搜索上述三种前缀

9. 在 `recipe/menu.c` 中登记此 `dish`

10. [编译、运行、测试 (how?)](./01-开发与构建.md)，若无问题可提交 Pull Request

<br>

## 开发准则

1. 代码高度可移植

2. Convention over Configuration

3. [NO UFO 原则: 不要乱丢文件到$HOME等目录，尤其是使用各种隐晦的文件名](https://www.yuque.com/ccmywish/blog/no-ufo)

    `chsrc` 主程序不提供配置文件，不提供数据文件，干净无污染。那么在实现 `recipe` 的时候，除了备份文件外，也不要污染用户环境。

## 其它

如果你借助 AI 进行开发，让 AI 阅读 [copilot-instructions.md](../.github/copilot-instructions.md) 并将它作为项目规范，该文件明确了很多在 AI 辅助开发时需要注意的问题。

<br>

[rawstr4c]: https://github.com/RubyMetric/rawstr4c
[chsrc-bootstrap]: ../bootstrap/
[recipe template]: ../src/recipe/recipe-template.c
[Wiki]: https://github.com/RubyMetric/chsrc/wiki
