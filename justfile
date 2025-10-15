#!/usr/bin/env just --justfile
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : justfile
# File Authors  : 曾奥然      <ccmywish@qq.com>
# Contributors  : Mikachu2333 <mikachu.23333@zohomail.com>
#               |
# Created On    : <2025-06-18>
# Last Modified : <2025-10-15>
#
# 该文件主要用于在原生Windows上执行项目的基本任务，而不借助于
# GNU make 以及相应的 MSYS2、Cygwin 环境
# --------------------------------------------------------------
# just (build)											-> chsrc.exe
# just bd (build-in-debug-mode)			-> chsrc-debug.exe
# just br (build-in-release-mode)		-> chsrc-release.exe
# just debug
# just test
# just clean
#
# just STATIC=1 br  	静态链接 (注意只能在 br 任务中使用)
#
# 不支持 just DEBUG=1，请直接使用 just bd (等价于 just build-in-debug-mode)
#
# --------------------------------------------------------------
# 注意，由于我们要在 GitHub Actions 上编译 x32 版的 chsrc，所以需要使用 make，
# 而不清楚 just 在 MINGW32 中的情况，所以我们在此 justfile 中并不实现关于 CI 的功能
# --------------------------------------------------------------

set windows-shell := ['cmd', '/c']

CC := if os() == 'windows' {
	"gcc"
} else if os() == 'macos' {
	"clang"
} else {
	"cc"
}

DEBUGGER := if os() == 'windows' {
	"gdb"
} else if os() == 'macos' {
	"lldb"
} else {
	"gdb"
}

CFLAGS_chk_Clang := if os() == 'windows' {
  if CC == 'clang' {
    '-target x86_64-pc-windows-gnu'
  } else {''}
} else {''}


CFLAGS_base := '-Iinclude -Ilib ' + CFLAGS_chk_Clang

WARN := '-Wall -Wextra -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -Wno-misleading-indentation' + ' ' + \
	'-Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare'
CFLAGS_warn := WARN


CFLAGS_debug := '-g -DXY_DEBUG'

DevMode-Target-Name     := 'chsrc'
DebugMode-Target-Name   := 'chsrc-debug'
ReleaseMode-Target-Name := 'chsrc-release'

STATIC := '0'

CFLAGS_static := "-static"

CFLAGS_chk_static := if STATIC == '1' {
	CFLAGS_static
} else { "" }


CFLAGS_optimization := "-O2"


CFLAGS_dev_mode_prompt 	   := CFLAGS_base
CFLAGS_debug_mode_prompt 	 := CFLAGS_base + ' ' + CFLAGS_debug
CFLAGS_release_mode_prompt := CFLAGS_base + ' ' + CFLAGS_optimization + ' ' + CFLAGS_chk_static

CFLAGS_dev_mode     := CFLAGS_dev_mode_prompt     + ' ' + CFLAGS_warn
CFLAGS_debug_mode   := CFLAGS_debug_mode_prompt   + ' ' + CFLAGS_warn
CFLAGS_release_mode := CFLAGS_release_mode_prompt + ' ' + CFLAGS_warn

#=======================

BIN_xy := if os() == 'windows' {'xy.exe'} else {'./xy'}
BIN_fw := if os() == 'windows' {'fw.exe'} else {'./fw'}
BIN_rm := if os() == 'windows' {'del'}    else {'rm'}
#=======================
alias b := build-in-dev-mode
alias bd:= build-in-debug-mode
alias br:= build-in-release-mode
alias build:=build-in-dev-mode
alias d := debug
alias t := test
alias check := test
alias c := clean

default: build-in-dev-mode

build-in-dev-mode:
	@echo Starting: Build in DEV mode: '{{CC}}' {{CFLAGS_dev_mode_prompt}} -o {{DevMode-Target-Name}}
	@{{CC}} src/chsrc-main.c {{CFLAGS_dev_mode}} -o {{DevMode-Target-Name}}
	@echo Finished: Build in DEV mode

build-in-debug-mode:
	@echo Starting: Build in DEBUG mode: '{{CC}}' {{CFLAGS_debug_mode_prompt}} -o {{DebugMode-Target-Name}}
	@{{CC}} src/chsrc-main.c {{CFLAGS_debug_mode}} -o {{DebugMode-Target-Name}}
	@echo Finished: Build in DEBUG mode

build-in-release-mode:
  @echo Starting: Build in RELEASE mode: '{{CC}}' {{CFLAGS_release_mode_prompt}} -o {{ReleaseMode-Target-Name}}
  @{{ if os() == 'windows' { \
        'if exist chsrc.res del chsrc.res' + \
        ' & windres src/resource/chsrc.rc -O coff -o chsrc.res' \
      } else { '' } }}
  @{{ if os() == 'windows' { \
      CC + ' src/chsrc-main.c chsrc.res ' + CFLAGS_release_mode + ' -o ' + ReleaseMode-Target-Name \
    } else { \
      CC + ' src/chsrc-main.c '           + CFLAGS_release_mode + ' -o ' + ReleaseMode-Target-Name \
    } }}
  @echo Finished: Build in RELEASE mode

debug: build-in-debug-mode
  @{{DEBUGGER}} {{DebugMode-Target-Name}}

test: test-xy test-fw

# 这两个测试文件都用 DEBUG mode
test-xy:
	@{{CC}} test/xy.c {{CFLAGS_debug_mode}} -o xy
	@{{BIN_xy}}

test-fw:
	@{{CC}} test/fw.c {{CFLAGS_debug_mode}} -o fw
	@{{BIN_fw}}

fastcheck:
	@perl ./test/cli.pl fastcheck

test-cli:
	@perl ./test/cli.pl

clean:
	-@{{BIN_rm}} *.exe
	-@{{BIN_rm}} xy
	-@{{BIN_rm}} fw
	-@{{BIN_rm}} chsrc
	-@{{BIN_rm}} chsrc-debug
	-@{{BIN_rm}} chsrc-release
	-@{{BIN_rm}} README.md.bak*

# 使用: just rawstr4c [--debug] Markdown.md
# rawstr4c *args:
#	@{{ if os() == 'windows' { 'tool\\rawstr4c\\run\\run.bat' } else { 'bash ./tool/rawstr4c/run/run.sh' } }} {{args}}
