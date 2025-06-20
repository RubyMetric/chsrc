#!/usr/bin/env just --justfile
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    :  justfile
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
#								|
# Created On    : <2025-06-18>
# Last Modified : <2025-06-20>
#
# just (build)
# just debug
# just test
# just clean
#
# just STATIC=1 # 静态链接
# just DEBUG=1  # 编译出 debug 版
#
# 该文件主要用于在原生Windows上执行项目的基本任务，而不借助于
# GNU make 以及相应的 MSYS2、Cygwin 环境
# --------------------------------------------------------------

set windows-shell := ['cmd', '/c']

CC := if os() == 'windows' {
	"gcc"
} else if os() == 'macos' {
	"clang"
} else {
	"cc"
}

DEBUGGER := 'gdb'

CFLAGS_base := '-Iinclude -Ilib'

CFLAGS_for_Clang := if os() == 'windows' {
  if CC == 'clang' {
    '-target x86_64-pc-windows-gnu'
  } else {''}
} else {''}


WARN := '-Wall -Wextra -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -Wno-misleading-indentation' + ' ' + \
	'-Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare'
CFLAGS_warn := WARN


DEBUG := '0'
CFLAGS_debug := if DEBUG != '0' { "-g" } else { "" }

Debuggable-Target-Name := 'chsrc-debug'

Target-Name := if DEBUG != '0' {
	Debuggable-Target-Name
} else {
	'chsrc'
}

CI := '0'
CI_ARTIFACT_NAME := 'chsrc'

STATIC := '0'

# 在 GitHub Actions 时的 Linux 环境下，just CI=1 时触发
CFLAGS_static := if STATIC == '1' {
	"-static"
} else if os() == 'linux' {
	if CI == '1' {"-static"} else {''}
} else {''}


CFLAGS := CFLAGS_base + ' ' + CFLAGS_debug + ' ' + CFLAGS_warn + ' ' + CFLAGS_static + ' ' + CFLAGS_for_Clang

CFLAGS_only_promp_for_dev := CFLAGS_base + ' ' + CFLAGS_debug + CFLAGS_static + CFLAGS_for_Clang

#=======================

BIN_xy := if os() == 'windows' {'xy.exe'} else {'./xy'}
BIN_fw := if os() == 'windows' {'fw.exe'} else {'./fw'}
BIN_rm := if os() == 'windows' {'del'}    else {'rm'}
#=======================
alias b := build
alias d := debug
alias t := test

default: build

build:
  @echo Starting: Compile chsrc executable
  @{{CC}} src/chsrc-main.c {{CFLAGS}} -o {{Target-Name}}
  @echo Finished: Compile chsrc executable using '{{CC}}' {{CFLAGS_only_promp_for_dev}} -o {{Target-Name}}

CI: build
	@mv {{Target-Name}} {{CI_ARTIFACT_NAME}}

debug:
  @{{DEBUGGER}} {{Debuggable-Target-Name}}

test: test-xy test-fw

test-xy:
	@{{CC}} test/xy.c {{CFLAGS}} -o xy
	@{{BIN_xy}}

test-fw:
	@{{CC}} test/fw.c {{CFLAGS}} -o fw
	@{{BIN_fw}}


check: test

fastcheck:
	@perl ./test/cli.pl fastcheck

test-cli:
	@perl ./test/cli.pl

clean:
	-@{{BIN_rm}} *.exe
	-@{{BIN_rm}} xy
	-@{{BIN_rm}} fw
	-@{{BIN_rm}} chsrc
	-@{{BIN_rm}} README.md.bak*
