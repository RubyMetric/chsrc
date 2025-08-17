# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : git-ignore-vscode-settings.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2025-08-17>
# Last Modified : <2025-08-17>
#
# VS Code 的某些操作/插件会频繁地修改我们的 .vscode/settings.json
# 文件，非常非常烦人，但是该文件又应该被加入仓库，所以我们使用以下
# 命令来忽略它在本地的变化
# ---------------------------------------------------------------

git update-index --skip-worktree .\.vscode\settings.json

# 恢复方法是:
# git update-index --no-skip-worktree .\.vscode\settings.json
