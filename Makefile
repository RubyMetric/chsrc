# ---------------------------------------------------------------
# File          : Makefile
# License       : GPLv3
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2023-08-28>
# Last modified : <2024-05-24>
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

Target = build/chsrc

CI_Build_Name = chsrc
#=======================

all: build_dir
	@$(CC) chsrc.c $(CFLAGS) -o $(Target)
	@echo; echo Compile done using \'$(CC)\' $(CFLAGS)

CI: all
	@mv $(Target) $(CI_Build_Name)

build_dir:
	@mkdir build

test: $(Target)
	./$(Target) list mirror
	./$(Target) list Target
	./$(Target) get  ruby
	./$(Target) get  python

test_xy: build_dir
	@$(CC) test/xy.c -o build/xy
	@./build/xy

clean:
	-@rm *.exe      2>/dev/null
	-@rm $(Target)  2>/dev/null
	-@rm ./build/* 2>/dev/null
