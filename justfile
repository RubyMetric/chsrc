#!/usr/bin/env just --justfile
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    :  justfile
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
#								|
# Created On    : <2025-06-18>
# Last Modified : <2025-06-18>
#
# just (build)
# just debug
# just test
#
# just DEBUG=1 # 编译出 debug 版
# --------------------------------------------------------------

set windows-shell := ['cmd', '/c']

CC := 'gcc'
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

# 在 GitHub Actions 时的 Linux 环境下，just CI=1 时触发
CFLAGS_static := if os() == 'linux' {
  if CI == '0' {
    "-static"
  } else {''}
} else {''}


CFLAGS := CFLAGS_base + ' ' + CFLAGS_debug + ' ' + CFLAGS_warn + ' ' + CFLAGS_static + ' ' + CFLAGS_for_Clang

CFLAGS_only_promp_for_dev := CFLAGS_base + ' ' + CFLAGS_debug + CFLAGS_static + CFLAGS_for_Clang

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
	@./xy

test-fw:
	@{{CC}} test/fw.c {{CFLAGS}} -o fw
	@./fw


check: test

fastcheck:
	@perl ./test/cli.pl fastcheck

test-cli:
	@perl ./test/cli.pl

clean:
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm fw     2>/dev/null
	-@rm chsrc  2>/dev/null
	-@rm README.md.bak* 2>/dev/null
