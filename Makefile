CFLAGS =

TARGET = chsrc
#=======================

all:
	@echo Compile
	@gcc chsrc.c $(CFLAGS) -o $(TARGET)

test:
	@gcc test_helper.c -o test
	@./test

test_cmd: $(TARGET)
	./$(TARGET) list mirror
	./$(TARGET) list target
	./$(TARGET) get  ruby
	./$(TARGET) get  python

clean:
	-@rm *.exe      2>/dev/null
	-@rm $(TARGET)  2>/dev/null
	-@rm ./test			2>/dev/null
