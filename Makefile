CFLAGS =

TARGET = target
#=======================

all:
	@echo Compile
	@gcc chsrc.c $(CFLAGS) -o $(TARGET)

test:
	@gcc test_helper.c -o test
	@./test

clean:
	-@rm *.exe      2>/dev/null
	-@rm $(TARGET)  2>/dev/null
	-@rm ./test			2>/dev/null
