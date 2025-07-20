:: ---------------------------------------------------------------
:: SPDX-License-Identifier: GPL-3.0-or-later
:: ---------------------------------------------------------------
:: File Name     : run.bat
:: File Authors  : Aoran Zeng <ccmywish@qq.com>
:: Contributors  :  Nul None  <nul@none.org>
:: Created On    : <2025-07-13>
:: Last Modified : <2025-07-21>
::
:: Run rawstr4c
::
:: Usage:
::
::   用户必须在代码根目录中运行此文件
::
::     %PREFIX%\run\run.bat [--debug] <Markdown.md>
::
::     %PREFIX%\run\run.bat [--debug] <Dir>
:: ---------------------------------------------------------------

@echo off
set PREFIX=tool\rawstr4c
REM -I 选项必须在文件前面
raku -I %PREFIX%\lib %PREFIX%\bin\rawstr4c %*
