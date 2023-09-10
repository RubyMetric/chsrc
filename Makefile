# ---------------------------------------------------------------
# File          : Makefile
# License       : GPLv3
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2023-08-28>
# Last modified : <2023-09-06>
# ---------------------------------------------------------------

CFLAGS = # -Wall

ifeq ($(OS), Windows_NT)
	CLANG_FLAGS = -target x86_64-pc-windows-gnu
endif

TARGET = chsrc
#=======================

all: echo
ifeq ($(CC), clang)
	@$(CC) chsrc.c $(CLANG_FLAGS) $(CFLAGS) -o $(TARGET)
else
	@$(CC) chsrc.c $(CFLAGS) -o $(TARGET)
endif

echo:
	@echo Compile using \'$(CC)\' $(CFLAGS) on $(OS) ...

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
