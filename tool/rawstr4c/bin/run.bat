:: ---------------------------------------------------------------
:: File Name     : run.bat
:: File Authors  : Aoran Zeng <ccmywish@qq.com>
:: Contributors  :  Nul None  <nul@none.org>
:: Created On    : <2025-07-13>
:: Last Modified : <2025-07-13>
::
:: Run rawstr4c.raku script
::
:: Usage:
::
::   用户必须在代码根目录中运行此文件
::
::     %PREFIX%\bin\run.bat [--debug] <Markdown.md>
::
::     %PREFIX%\bin\run.bat [--debug] <Dir>
:: ---------------------------------------------------------------

@echo off
set PREFIX=tool\rawstr4c
REM -I 选项必须在文件前面
raku -I %PREFIX%\lib %PREFIX%\rawstr4c.raku %*
