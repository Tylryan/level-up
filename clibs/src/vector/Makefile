LIB_NAME=vector
TEST_DIR=test

#all: $(LIB_NAME)

#$(LIB_NAME): objects
#gcc -g $^ -o $@

archive: objects
	ar rcsv lib$(LIB_NAME).a $(LIB_NAME).o
	mv lib$(LIB_NAME).a ../../libs
	cp $(LIB_NAME).h ../../include

objects: $(LIB_NAME).c
	gcc -g -c $^
run: all
	./$(LIB_NAME)

test: objects
	make -C $(TEST_DIR)
clean:
	rm  ../../libs/lib$(LIB_NAME).a
	rm ../../include/$(LIB_NAME).h
	rm *.o
