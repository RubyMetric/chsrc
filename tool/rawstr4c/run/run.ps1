# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : run.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-21>
#
# Run rawstr4c
#
# Usage:
#
#   用户必须在代码根目录中运行此文件
#
#     $PREFIX\run\run.ps1 [--debug] <Markdown.md>
#
#     $PREFIX\run\run.ps1 [--debug] <Dir>
# ---------------------------------------------------------------

$PREFIX = "tool/rawstr4c"

# -I 选项必须在文件前面
raku  -I $PREFIX\lib $PREFIX\bin\rawstr4c $args
