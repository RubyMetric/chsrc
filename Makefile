#!/usr/bin/make -f
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    :   Makefile
# File Authors  :  Aoran Zeng   <ccmywish@qq.com>
# Contributors  :  Yangmoooo    <yangmoooo@outlook.com>
#               | sanchuanhehe  <wyihe5520@gmail.com>
#               | Mikachu2333   <mikachu.23333@zohomail.com>
#               |
# Created On    : <2023-08-28>
# Last Modified : <2025-10-11>
#
# 请阅读 ./doc/01-开发与构建.md 来使用
# --------------------------------------------------------------

#=========== OS Check ================
On-Linux = 0
On-Windows = 0
On-macOS = 0

# Windows 会定义 OS 或 ComSpec 环境变量
ifdef ComSpec
	On-Windows = 1
else ifdef OS
	ifeq ($(OS), Windows_NT)
		On-Windows = 1
	endif
endif

# 只在非 Windows 环境下调用 uname
ifneq ($(On-Windows), 1)
	UNAME_S := $(shell uname 2>/dev/null || echo unknown)
	ifeq ($(UNAME_S), Linux)
		On-Linux = 1
	endif
	ifeq ($(UNAME_S), Darwin)
		On-macOS = 1
	endif
endif
#=====================================



#======== Default Tooling ============
ifeq ($(On-Windows), 1)
  # Windows 环境 - 使用 gcc
	CC = gcc
else ifeq ($(On-macOS), 1)
	CC = clang
else
	CC = cc
endif

ifeq ($(On-macOS), 1)
	DEBUGGER = lldb
else
	DEBUGGER = gdb
endif
#=====================================



#======== Compilation Config ==========
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

DevMode-Target-Name = chsrc
DebugMode-Target-Name = chsrc-debug
ReleaseMode-Target-Name = chsrc-release
CIReleaseMode-Target-Name = chsrc-ci-release

CFLAGS_debug  = -g -DXY_DEBUG
CFLAGS_static = -static
CFLAGS_optimization = -O2

ifdef DEBUG
	CFLAGS += $(CFLAGS_debug)
endif

STATIC = 0

ifeq ($(STATIC), 1)
	CFLAGS += $(CFLAGS_static)
endif
#=====================================



#====== CI release mode 的配置 =======
ifeq ($(MAKECMDGOALS), build-in-ci-release-mode)

	CFLAGS += $(CFLAGS_optimization)

  # 仅在 Linux 上使用静态链接
	ifeq ($(On-Linux), 1)
		CFLAGS += $(CFLAGS_static)
	endif

  # GitHub Actions 上的 macOS 中的 LLVM 太老了
  # 而且 gcc 被重命名为了 clang
  # 需要直接指定版本
  ifeq ($(On-macOS), 1)
    CC = gcc-14
  endif

endif
#=====================================



#============ Aliases ================
all: build

b: build-in-dev-mode
build: build-in-dev-mode
bd: build-in-debug-mode
br: build-in-release-mode
bcir: build-in-ci-release-mode
d: debug
t: test
check: test
c: clean
#=====================================



build-in-dev-mode:
	@echo Starting: Build in DEV mode: \'$(CC)\' $(CFLAGS) -o $(DevMode-Target-Name)
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(DevMode-Target-Name)
	@echo Finished: Build in DEV mode

build-in-debug-mode: CFLAGS += $(CFLAGS_debug)
build-in-debug-mode:
	@echo Starting: Build in DEBUG mode: \'$(CC)\' $(CFLAGS) -o $(DebugMode-Target-Name)
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(DebugMode-Target-Name)
	@echo Finished: Build in DEBUG mode

build-in-release-mode: CFLAGS += $(CFLAGS_optimization)
build-in-release-mode:
	@echo Starting: Build in RELEASE mode: \'$(CC)\' $(CFLAGS) -o $(ReleaseMode-Target-Name)
ifeq ($(On-Windows), 1)
	@if exist lib\chsrc.res del /Q lib\chsrc.res 2>nul
	@windres lib\win_res.rc -O coff -o lib\chsrc.res -Iinclude -Ilib
	@$(CC) src/chsrc-main.c lib/chsrc.res $(CFLAGS) $(_C_Warning_Flags) -o $(ReleaseMode-Target-Name)
	@del /Q lib\chsrc.res 2>nul
else
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(ReleaseMode-Target-Name)
endif
	@echo Finished: Build in RELEASE mode

# CI release mode 的配置在该文件上方
build-in-ci-release-mode:
	@echo Starting: Build in CI-RELEASE mode: \'$(CC)\' $(CFLAGS) -o $(CIReleaseMode-Target-Name)
ifeq ($(On-Windows), 1)
	@windres lib\win_res.rc -O coff -o lib\chsrc.res -Iinclude -Ilib
	@$(CC) src/chsrc-main.c lib/chsrc.res $(CFLAGS) $(_C_Warning_Flags) -o $(CIReleaseMode-Target-Name)
else
	@$(CC) src/chsrc-main.c $(CFLAGS) $(_C_Warning_Flags) -o $(CIReleaseMode-Target-Name)
endif
	@echo Finished: Build in CI-RELEASE mode

# 永远重新编译
debug: build-in-debug-mode
	@$(DEBUGGER) $(DebugMode-Target-Name)

test: test-make-env test-xy test-fw

test-make-env:
	@echo "On-Linux: $(On-Linux)"
	@echo "On-Windows: $(On-Windows)"
	@echo "On-macOS: $(On-macOS)"
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
ifeq ($(On-Windows), 1)
	@echo "USER: $(USERNAME)"
	@echo "PWD: $(CURDIR)"
else
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
endif

# 这两个测试文件都用 DEBUG mode
test-xy: CFLAGS += $(CFLAGS_debug)
test-xy:
ifeq ($(On-Windows), 1)
	@$(CC) test/xy.c $(CFLAGS) -o xy.exe
	@xy.exe
else
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy
endif

test-fw: CFLAGS += $(CFLAGS_debug)
test-fw:
ifeq ($(On-Windows), 1)
	@$(CC) test/fw.c $(CFLAGS) -o fw.exe
	@fw.exe
else
	@$(CC) test/fw.c $(CFLAGS) -o fw
	@./fw
endif

check: test

# AUR package 安装时将执行此 target
fastcheck: $(DevMode-Target-Name)
	@perl ./test/cli.pl fastcheck

test-cli: $(DevMode-Target-Name)
	@perl ./test/cli.pl

clean:
ifeq ($(On-Windows), 1)
	-@if exist *.exe del /Q *.exe 2>nul
	-@if exist xy.exe del /Q xy.exe 2>nul
	-@if exist fw.exe del /Q fw.exe 2>nul
	-@if exist README.md.bak* del /Q README.md.bak* 2>nul
	-@if exist chsrc.exe del /Q chsrc.exe 2>nul
	-@if exist chsrc-debug.exe del /Q chsrc-debug.exe 2>nul
	-@if exist chsrc-release.exe del /Q chsrc-release.exe 2>nul
	-@if exist chsrc-ci-release.exe del /Q chsrc-ci-release.exe 2>nul
else
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm fw     2>/dev/null
	-@rm README.md.bak*    2>/dev/null
	-@rm chsrc  					 2>/dev/null
	-@rm chsrc-debug       2>/dev/null
	-@rm chsrc-release  	 2>/dev/null
	-@rm chsrc-ci-release  2>/dev/null
endif

# -include pkg/deb/Makefile # 不这么做，因为 pkg/deb/Makefile 需要在 pkg/deb 目录下执行
# 保持动词在前的任务名风格
build-deb:
	@$(MAKE) -C pkg/deb deb-build

clean-deb:
	@$(MAKE) -C pkg/deb deb-clean

install: $(ReleaseMode-Target-Name)
	install -D -m 755 $(ReleaseMode-Target-Name) $(DESTDIR)/usr/bin/chsrc
	install -D -m 644 doc/chsrc.1 $(DESTDIR)/usr/share/man/man1/chsrc.1

# 这样还是太麻烦，不用，我们还是靠 just 来调用吧
#
# 通过 make rawstr4c ARGS="[--debug] Markdown.md" 来调用
#rawstr4c:
#	@bash ./tool/rawstr4c/run/run.sh $(ARGS)

.PHONY: all b build bd br bcir d t check c \
	build-in-dev-mode build-in-debug-mode build-in-release-mode build-in-ci-release-mode \
	debug test test-make-env test-xy test-fw fastcheck test-cli clean install build-deb clean-deb rawstr4c
