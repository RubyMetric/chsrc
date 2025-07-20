<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : (rawstr4c introduction).md
 ! Doc Authors   : Aoran Zeng <ccmywish@qq.com>
 ! Contributors  :  Nul None  <nul@none.org>
 !               |
 ! Created On    : <2025-07-12>
 ! Last Modified : <2025-07-21>
 ! ---------------------------------------------------------- -->

# rawstr4c

Use this tool when you need to write complex C language strings.

```bash
$ rawstr4c --help
```

<br>



## Convention

A configuration file should use this order:

1. section title
2. description of the variable
3. configuration block (configblock)
4. configuration block (configblock) comments
5. code block (codeblock) (raw string)
6. comments for the content of the code block (codeblock)

<br>



## Configuration Syntax

```markdown

- config-item1 = `:mode`

- config-item2 = `true|false|yes|no`

- config-item3 = `string value`

```

Configuration items always start with `-`, followed by the configuration item name and an `=`, and the right-hand value must be wrapped with ``` `` ```.

Note: if the value is not arbitrarily given by the user, it should be set as a mode type, using `:` as a prefix.

<br>



## Configuration Items

Note: unless otherwise specified, the first item is the default value

- output =

  - `:terminal` = output to terminal
  - `:macro` = output as a `.h` file, defined as macro
  - `:global-variable` = output a `.h` file and corresponding `.c` file, defined as global variable
  - `:global-variable-only-header` = output only as a `.h` file, defined as global variable

- ~~output-file~~ = (not yet implemented)

  Custom generated header filename, default value is `rawstr4c.h`

- translate =

  - `:escape` = escape only
  - `:oct` = octal
  - `:hex` = hexadecimal

- postfix =

  - `:use-language` = use the language of the codeblock
  - `your string` = use a custom string as suffix

- name =

  Generated variable name, will include prefix and suffix by default. If this configuration item is not given, the section title will be used

- name-literally = `false` | `true`

  Ignore other configuration items and directly use `name` as the variable name

- namespace =

  Will serve as a prefix after `prefix` and before variable name `name`, affecting the next level section

- keep-prefix = `true` | `false`

  Whether the variable name uses prefix

- keep-postfix = `true` | `false`

  Whether the variable name uses postfix
