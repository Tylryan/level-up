CC       = gcc
FLAGS	 = -Wall -Wextra -Wpedantic
LIB_NAME = str
LIB_PATH = ../../libs
INCLUDE_PATH = ../../include
TEST_DIR=test

archive: objects
	ar rcsv lib$(LIB_NAME).a $(LIB_NAME).o
	mv lib$(LIB_NAME).a ../../libs
	cp $(LIB_NAME).h ../../include

install: objects
	cp ../../libs/lib$(LIB_NAME).a /usr/local/lib
	cp ../../include/$(LIB_NAME).h /usr/local/include

uninstall:
	rm /usr/local/lib/lib$(LIB_NAME).a
	rm /usr/local/include/$(LIB_NAME).h

objects: $(LIB_NAME).c
	gcc -g -c $^ $(FLAGS) -O2

# Currently this doesn't work
test: $(LIB_NAME).c $(LIB_NAME).h $(TEST_DIR)/main.c
	make -C $(TEST_DIR)
	./$(TEST_DIR)/main.c
clean:
	rm  ../../libs/lib$(LIB_NAME).a
	rm ../../include/$(LIB_NAME).h
	rm *.o
