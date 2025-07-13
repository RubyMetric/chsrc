# ---------------------------------------------------------------
# File Name     : rawstr4c.ps1
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-13>
#
# Run rawstr4c.raku script
#
# Usage:
#
#   用户必须在代码根目录中运行此文件
#
#     .\rawstr4c\bin\rawstr4c.ps1 [--debug] <Markdown.md>
#
#     .\rawstr4c\bin\rawstr4c.ps1 [--debug] <Dir>
# ---------------------------------------------------------------

# -I 选项必须在文件前面
raku  -I .\rawstr4c\lib .\rawstr4c\rawstr4c.raku $args
