# ---------------------------------------------------------------
# File          : Makefile
# License       : GPLv3
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2023-08-28>
# Last modified : <2023-09-20>
# ---------------------------------------------------------------

CFLAGS = # -Wall

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

TARGET = chsrc

CI_BUILD_NAME = chsrc
#=======================

all:
	@$(CC) chsrc.c $(CFLAGS) -o $(TARGET)
	@echo; echo Compile done using \'$(CC)\' $(CFLAGS)

CI: all
	@mv chsrc $(CI_BUILD_NAME)

test: $(TARGET)
	./$(TARGET) list mirror
	./$(TARGET) list target
	./$(TARGET) get  ruby
	./$(TARGET) get  python

.PHONY: xy
xy:
	@$(CC) test_xy.c -o xy
	@./xy

clean:
	-@rm *.exe      2>/dev/null
	-@rm $(TARGET)  2>/dev/null
	-@rm ./xy	  		2>/dev/null
