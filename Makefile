CFLAGS =

TARGET = target
#=======================

all:
	@echo Compile
	@gcc chsrc.c $(CFLAGS) -o $(TARGET)
