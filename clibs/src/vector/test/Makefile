LIB_NAME=vector
TEST_DIR=test
LIBS=-l$(LIB_NAME) -lctest

all:
	gcc -g *.c -o test -I../../../include -L../../../libs $(LIBS)
	echo "TESTING"
	./test
