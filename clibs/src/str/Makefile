CC       = gcc
FLAGS	 = -Wall -Werror -Wextra -Wpedantic
LIB_NAME = str
LIB_PATH = ../../libs
INCLUDE_PATH = ../../include

all: a.out

a.out: str.o
	$(CC) $^ $(FLAGS) -g

str.o: str.c
	gcc $^ -c $(FLAGS) -g

clean:
	rm a.out *.o *.a
	rm $(LIB_PATH)/lib$(LIB_NAME).a $(INCLUDE_PATH)/$(LIB_NAME).h

arc:
	ar cr lib$(LIB_NAME).a str.o
	cp lib$(LIB_NAME).a $(LIB_PATH)
	cp $(LIB_NAME).h $(INCLUDE_PATH)
