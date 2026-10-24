<!-- -----------------------------------------------------------
 ! SPDX-License-Identifier: GFDL-1.3-or-later
 ! -------------------------------------------------------------
 ! Doc Type      : Markdown
 ! Doc Name      : (Shell Completion Introduction).md
 ! Doc Authors   : @swim233
 ! Contributors  : TheSw1m <swim853279614@163.com>
 !               |
 ! Created On    : <2026-09-09>
 ! Last Modified : <2026-09-09>
 !
 ! Shell 补全目录说明
 ! ---------------------------------------------------------- -->

# Shell 补全说明

本目录包含 chsrc 的 Shell 补全实现：

- `bash_completion.sh`：Bash 补全
- `_chsrc`：原生 Zsh 补全

Zsh 会通过 `#compdef chsrc` 将 `_chsrc` 注册为 `chsrc` 的补全函数。补全
函数根据 Zsh 提供的 `words`、`CURRENT` 和 `PREFIX` 判断光标位置及当前待
补全内容。

## `_describe`

`_describe` 是 Zsh 补全系统提供的辅助函数，用于批量添加带说明的候选项。
本项目使用 `候选项:说明` 格式的数组，使补全菜单可以同时展示命令、选项
及其用途；`-t` 参数用于为候选项指定标签，方便 Zsh 按类别应用样式。

## `compadd`

`compadd` 是直接向当前补全上下文添加候选项的内建命令。本项目用它添加
`-scope=`：`-S ''` 禁止在候选项后自动附加空格，使用户可以继续输入作用域；
`-X` 则为这一组候选项提供说明。

## `compset`

`compset` 用于调整 Zsh 的补全上下文。本项目使用 `compset -P '-scope='`
把已输入的 `-scope=` 从 `PREFIX` 移到 `IPREFIX`。这样后续匹配只针对
`default`、`project`、`user`、`system`，插入结果时仍会保留 `-scope=`
前缀。
