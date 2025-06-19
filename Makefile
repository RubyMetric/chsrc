#!/usr/bin/make -f
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : Makefile
# File Authors  :  Aoran Zeng  <ccmywish@qq.com>
# Contributors  :  Yangmoooo   <yangmoooo@outlook.com>
#								| sanchuanhehe <wyihe5520@gmail.com>
#								|
# Created On    : <2023-08-28>
# Last Modified : <2025-06-19>
# --------------------------------------------------------------

On-Linux = 0
On-Windows = 0
On-macOS = 0

ifeq ($(shell uname), Linux)
	On-Linux = 1
endif

# 只有Windows会定义$(OS)变量
ifeq ($(OS), Windows_NT)
	On-Windows = 1
endif
#=======================

CFLAGS += -Iinclude -Ilib

ifeq ($(On-Windows), 1)
	CLANG_FLAGS = -target x86_64-pc-windows-gnu
endif

ifeq ($(CC), clang)
	CFLAGS += $(CLANG_FLAGS)
endif

override WARN += -Wall -Wextra -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -Wno-misleading-indentation \
	-Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare
_C_Warning_Flags := $(WARN)

Target-Name = chsrc

Debuggable-Target-Name = chsrc-debug

ifdef DEBUG
	CFLAGS += -g
	Target-Name = $(Debuggable-Target-Name)
endif

DEBUGGER = gdb

STATIC = 0
#=======================

# 由 GitHub Actions 在调用时修改
CI_ARTIFACT_NAME = chsrc

ifeq ($(MAKECMDGOALS), CI)
	ifeq ($(On-Linux), 1)
		STATIC = 1
	endif
endif
#=======================


all:
ifeq ($(STATIC), 1)
CFLAGS += -static
endif

all:
	@echo "Starting: Compile chsrc executable"
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(Target-Name)
	@echo Finished: Compile chsrc executable using \'$(CC)\' $(CFLAGS)

CI: all
	@mv $(Target-Name) $(CI_ARTIFACT_NAME)


debug: CFLAGS += -g
debug: Target-Name = $(Debuggable-Target-Name)
debug: all
	@$(DEBUGGER) $(Debuggable-Target-Name)

test: test-make-env test-xy test-fw

test-make-env:
	@echo "On-Linux: $(On-Linux)"
	@echo "On-Windows: $(On-Windows)"
	@echo "On-macOS: $(On-macOS)"
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "Target-Name: $(Target-Name)"
	@echo "USER: $$(whoami)"
	@echo "PWD: $(shell pwd)"
	@echo "UID: $$(id -u)"
	@echo "GID: $$(id -g)"
# 检查HOME环境变量
	@if [ -z "$(HOME)" ]; then \
	 echo "HOME environment variable is not set!"; \
	else \
	 echo "HOME: $(HOME)"; \
	fi

test-xy:
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy

test-fw:
	@$(CC) test/fw.c $(CFLAGS) -o fw
	@./fw


check: test

# AUR package 安装时将执行此 target
fastcheck: $(Target-Name)
	@perl ./test/cli.pl fastcheck

test-cli: $(Target-Name)
	@perl ./test/cli.pl

clean:
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm fw     2>/dev/null
	-@rm chsrc  2>/dev/null
	-@rm README.md.bak* 2>/dev/null

# -include pkg/deb/Makefile # 不这么做，因为 pkg/deb/Makefile 需要在 pkg/deb 目录下执行
# 保持动词在前的任务名风格
build-deb:
	@$(MAKE) -C pkg/deb deb-build

clean-deb:
	@$(MAKE) -C pkg/deb deb-clean

install: $(Target-Name)
	install -D -m 755 $(Target-Name) $(DESTDIR)/usr/bin/$(Target-Name)
	install -D -m 644 doc/chsrc.1 $(DESTDIR)/usr/share/man/man1/chsrc.1

.PHONY: all CI debug check test test-make-env test-xy test-fw fastcheck test-cli clean install build-deb clean-deb
