# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : Makefile
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  : Yangmoooo  <yangmoooo@outlook.com>
#								|
# Created On    : <2023-08-28>
# Last Modified : <2024-12-27>
# --------------------------------------------------------------

CFLAGS += -Iinclude -Ilib # -Wall

# 只有Windows会定义该变量
ifeq ($(OS), Windows_NT)
	CLANG_FLAGS = -target x86_64-pc-windows-gnu
endif
ifeq ($(CC), clang)
	CFLAGS += $(CLANG_FLAGS)
endif
ifeq ($(shell uname), Linux)
	CFLAGS += -static
endif
ifdef DEBUG
	CFLAGS += -g
endif

override WARN += -Wall -Wextra -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -Wno-misleading-indentation \
	-Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare
_C_Warning_Flags := $(WARN)

Target = chsrc

CI_Build_Name = chsrc
#=======================

all:
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(Target)
	@echo; echo Compile done using \'$(CC)\' $(CFLAGS)

CI: all
	@mv $(Target) $(CI_Build_Name)

debug: CFLAGS += -g
debug: all
	@gdb ./chsrc

test: test-xy test-fw

test-xy:
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy

test-fw:
	@$(CC) test/fw.c $(CFLAGS) -o fw
	@./fw

# AUR package 安装时将执行此 target
fastcheck: $(Target)
	@perl ./test/cli.pl fastcheck

test-cli: $(Target)
	@perl ./test/cli.pl

clean:
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm fw     2>/dev/null
	-@rm chsrc  2>/dev/null
	-@rm README.md.bak* 2>/dev/null
