CFLAGS =

TARGET = chsrc
TEST_TARGET = test_$(TARGET)
#=======================

all:
	@echo Compile
	@gcc chsrc.c $(CFLAGS) -o $(TARGET)

test:
	@gcc test_helper.c -o $(TEST_TARGET)
	@./$(TEST_TARGET)

test_cmd: $(TARGET)
	./$(TARGET) list mirror
	./$(TARGET) list target
	./$(TARGET) get  ruby
	./$(TARGET) get  python

clean:
	-@rm *.exe      2>/dev/null
	-@rm $(TARGET)  2>/dev/null
	-@rm ./test			2>/dev/null
