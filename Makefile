# ------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# -------------------------------------------------------------
# Build File    : Makefile
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2023-08-28>
# Last Modified : <2024-09-10>
# ---------------------------------------------------------------

CFLAGS = -Iinclude # -Wall

# 只有Windows会定义该变量
ifeq ($(OS), Windows_NT)
	CLANG_FLAGS = -Target x86_64-pc-windows-gnu
endif
ifeq ($(CC), clang)
	CFLAGS += $(CLANG_FLAGS)
endif
ifeq ($(shell uname), Linux)
	CFLAGS += -static
endif

Target = chsrc

CI_Build_Name = chsrc
#=======================

all:
	@$(CC) src/chsrc-main.c $(CFLAGS) -o $(Target)
	@echo; echo Compile done using \'$(CC)\' $(CFLAGS)

CI: all
	@mv $(Target) $(CI_Build_Name)

test: $(Target)
	@perl ./test/cli.pl

fastcheck: $(Target)
	@perl ./test/cli.pl fastcheck

test-xy:
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy

clean:
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm chsrc  2>/dev/null
