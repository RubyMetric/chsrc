# ---------------------------------------------------------------
# File          : Makefile
# License       : GPLv3
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2023-08-28>
# Last modified : <2024-05-25>
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
	@$(CC) src/chsrc.c $(CFLAGS) -o $(Target)
	@echo; echo Compile done using \'$(CC)\' $(CFLAGS)

CI: all
	@mv $(Target) $(CI_Build_Name)

test: $(Target)
	./$(Target) list mirror
	./$(Target) list target
	./$(Target) get  ruby
	./$(Target) get  python

test_xy:
	@$(CC) test/xy.c $(CFLAGS) -o xy
	@./xy

clean:
	-@rm *.exe  2>/dev/null
	-@rm xy     2>/dev/null
	-@rm chsrc  2>/dev/null
