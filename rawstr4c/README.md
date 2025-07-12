<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : (rawstr4c configuration).md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2025-07-12>
 ! Last Modified : <2025-07-12>
 ! ---------------------------------------------------------- -->

# rawstr4c

在你需要写一个复杂的C语言字符串的时候，请想起我... \" / \n ' \\ `

<br>

## 配置语法

```markdown

- config-item = `value`

- config-item2 = `:mode`

```

配置项总是以 `-` 开头，配置项名称后跟一个 `=`，右值必须使用 ``` `` ``` 来包裹

注意，如果值并非是由用户随意给出，而是只能是默认的几种值之一，则使用 `:` 作为前缀

<br>



## 配置项

- output =

  - `:terminal` = 在终端输出
  - `:global-variable` = 输出为一个 `.h` 文件，定义为全局变量
  - `:macro` = 输出为一个 `.h` 文件，定义为宏

- output-file =

  自定义生成的头文件名

- translate =

  - `:oct` = 八进制
  - `:hex` = 十六进制
  - `:escape` = 仅转义

- postfix =

  - `:use-language`: 使用 code block 的语言
  - `scratch string`: 使用某一自定义字符串为后缀

- name =

  生成的变量名，默认将会包含前缀和后缀

- name-literally = `true` | `false`

  无视其他配置项，直接使用 `name` 作为变量名

- keep-prefix =

  变量名是否使用前缀

- keep-postfix =

  变量名是否使用后缀
