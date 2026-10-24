#!/usr/bin/make -f
# --------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# --------------------------------------------------------------
# Build File    : Makefile
# File Authors  : @ccmywish
# Contributors  : @Yangmoooo
#								| @sanchuanhehe
#								| @G_I_Y
#               | @NewbieXvwu
#               | @Mikachu2333
#               | @swim233
#               |
# Created On    : <2023-08-28>
# Last Modified : <2026-09-09>
#
# 请阅读 ./doc/01-开发与构建.md 来使用
# --------------------------------------------------------------

#=========== OS Check ================
On-Linux = 0
On-Windows = 0
On-macOS = 0

ifeq ($(shell uname), Linux)
	On-Linux = 1
endif

ifeq ($(shell uname), Darwin)
	On-macOS = 1
endif

# 只有 MSYS2 会定义 $(OS) 变量，而原生 Windows 不会定义
# 而 原生 Windows 会定义 $(ComSpec) 变量，且区分大小写，但是 MSYS2 并不会定义
ifeq ($(OS), Windows_NT)
	On-Windows = 1
endif
#=====================================



#======== Default Tooling ============
ifeq ($(On-Windows), 1)
  # MSYS2 环境
	CC = cc
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
  # 依据 MSYS2 环境的目标架构选择 clang 的 target triple
  # MSYSTEM_CARCH 由 MSYS2 提供: x86_64 (MINGW64/UCRT64/CLANG64) / i686 (MINGW32) / aarch64 (CLANGARM64)
  # 非 MSYS2 环境 (独立 LLVM) 下回退到 x86_64
	Windows-Clang-Arch = $(if $(MSYSTEM_CARCH),$(MSYSTEM_CARCH),x86_64)
	CFLAGS_clang = -target $(Windows-Clang-Arch)-pc-windows-gnu
endif

ifeq ($(CC), clang)
	CFLAGS += $(CFLAGS_clang)
endif

override WARN += -Wall -Wextra -Wno-unused-variable -Wno-unused-function -Wno-missing-braces -Wno-misleading-indentation \
	-Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare

# 将在后续的 target 定义中看到，我们是单独使用这个变量，而不是 += 到 CFLAGS 中
# 是因为想在用户 make 某个 target 的时候，不显示出一长串警告的编译选项
# 而只显示出对用户重要的、便于排查问题的编译选项
CFLAGS_warning := $(WARN)

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



#====== 两种 release mode 的共同配置 ==========
ifneq ($(filter build-in-release-mode br build-in-ci-release-mode bcir,$(MAKECMDGOALS)),)

  CFLAGS += $(CFLAGS_optimization)

	ifeq ($(On-Windows), 1)
    # 注意: Android 交叉编译同样在 Windows/MSYS2 下进行, 必须排除, 不能链入 Windows 资源
		ifneq ($(CROSS_BUILD_WINDOWS_FOR_ANDROID), 1)
			Use-Binary-Windows-Resource = 1
		endif
	endif

endif
#=====================================



#====== CI release mode 的配置 =======
ifneq ($(filter build-in-ci-release-mode bcir,$(MAKECMDGOALS)),)

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






#============ Target 定义开始 ================

# 编译开始结束时的提示信息
Starting-Echo-Info = echo Starting: Build in ${1} mode: \'$(CC)\' $(CFLAGS) -o ${2}
Finished-Echo-Info = echo Finished: Build in ${1} mode

# 真正的编译命令
Build-Command = $(CC) src/chsrc-main.c $(CFLAGS) $(CFLAGS_warning) -o $(1)

Build-Command-For-Release = $(CC) src/chsrc-main.c ${Windows-Res-File} $(CFLAGS) $(CFLAGS_warning) -o $(1)



build-in-dev-mode:
	@$(call Starting-Echo-Info, DEV, $(DevMode-Target-Name))
	@$(call Build-Command, $(DevMode-Target-Name))
	@$(call Finished-Echo-Info, DEV)

# 这是 Target-specific variables
build-in-debug-mode: CFLAGS += $(CFLAGS_debug)
build-in-debug-mode:
	@$(call Starting-Echo-Info, DEBUG, $(DebugMode-Target-Name))
	@$(call Build-Command, $(DebugMode-Target-Name))
	@$(call Finished-Echo-Info, DEBUG)



ifeq ($(Use-Binary-Windows-Resource), 1)

  Windows-Res-File = chsrc.res

# 动态增加 target: Windows 上的二进制资源文件
${Windows-Res-File}:
	@windres src/resource/chsrc.rc -O coff -o ${Windows-Res-File}

# 动态增加先决条件
build-in-release-mode build-in-ci-release-mode: ${Windows-Res-File}
endif

build-in-release-mode:
	@$(call Starting-Echo-Info, RELEASE, $(ReleaseMode-Target-Name))
	@$(call Build-Command-For-Release,   $(ReleaseMode-Target-Name))
	@$(call Finished-Echo-Info, RELEASE)

# 确保从干净源码树执行 install 时已构建 RELEASE mode 二进制
$(ReleaseMode-Target-Name): build-in-release-mode

build-in-ci-release-mode:
	@$(call Starting-Echo-Info, CI-RELEASE, $(CIReleaseMode-Target-Name))
	@$(call Build-Command-For-Release,      $(CIReleaseMode-Target-Name))
	@$(call Finished-Echo-Info, CI-RELEASE)



# 永远重新编译
debug: build-in-debug-mode
	@$(DEBUGGER) $(DebugMode-Target-Name)



test: test-make-var test-xy test-fw test-zsh-completion

test-make-var:
	@echo "On-Linux: $(On-Linux)"
	@echo "On-Windows: $(On-Windows)"
	@echo "On-macOS: $(On-macOS)"
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
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

# 这两个测试文件都用 DEBUG mode
test-xy: CFLAGS += $(CFLAGS_debug)
test-xy:
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy

test-fw: CFLAGS += $(CFLAGS_debug)
test-fw:
	@$(CC) test/fw.c $(CFLAGS) -o fw
	@./fw

test-zsh-completion:
	@perl ./test/verify-sync-state-of-zsh-completion
	@if command -v zsh >/dev/null 2>&1; then \
	  zsh -n ./tool/completion/_chsrc ./test/simulate-zsh-completion.zsh ./test/zsh-completion-integration.zsh && \
	  zsh -f ./test/simulate-zsh-completion.zsh && \
	  zsh -f ./test/zsh-completion-integration.zsh; \
	elif [ "$(REQUIRE_ZSH)" = "1" ]; then \
	  echo "zsh is required for Zsh completion tests" >&2; \
	  exit 1; \
	else \
	  echo "zsh not found; skipped Zsh completion runtime tests"; \
	fi

check: test

# AUR package 安装时将执行此 target
fastcheck: $(DevMode-Target-Name)
	@perl ./test/cli.pl fastcheck

test-cli: $(DevMode-Target-Name)
	@perl ./test/cli.pl

clean:
	-@rm *.exe  2>/dev/null
	-@rm *.res  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm fw     2>/dev/null
	-@rm README.md.bak*    2>/dev/null

	-@rm chsrc  					 2>/dev/null
	-@rm chsrc-debug       2>/dev/null
	-@rm chsrc-release  	 2>/dev/null
	-@rm chsrc-ci-release  2>/dev/null



# -include pkg/deb/Makefile # 不这么做，因为 pkg/deb/Makefile 需要在 pkg/deb 目录下执行
# 保持动词在前的任务名风格
build-deb:
	@$(MAKE) -C pkg/deb deb-build

clean-deb:
	@$(MAKE) -C pkg/deb deb-clean

# 源码安装使用 Zsh 通用的 site-functions 目录；Debian 打包时会将其
# 覆盖为 /usr/share/zsh/vendor-completions，以符合 Debian 的目录约定
ZSH_COMPLETION_DIR ?= /usr/share/zsh/site-functions

install: $(ReleaseMode-Target-Name)
	install -D -m 755 $(ReleaseMode-Target-Name) $(DESTDIR)/usr/bin/chsrc
	install -D -m 644 doc/chsrc.1 $(DESTDIR)/usr/share/man/man1/chsrc.1
	install -D -m 644 tool/completion/bash_completion.sh  $(DESTDIR)/usr/share/bash-completion/completions/chsrc
	install -D -m 644 tool/completion/_chsrc $(DESTDIR)$(ZSH_COMPLETION_DIR)/_chsrc



# 这样还是太麻烦，不用，我们还是靠 just 来调用吧
#
# 通过 make rawstr4c ARGS="[--debug] Markdown.md" 来调用
#rawstr4c:
#	@bash ./tool/rawstr4c/run/run.sh $(ARGS)

.PHONY: all b build bd br bcir d t check c \
	build-in-dev-mode build-in-debug-mode build-in-release-mode build-in-ci-release-mode \
	debug test test-make-var test-xy test-fw test-zsh-completion fastcheck test-cli clean install build-deb clean-deb rawstr4c
