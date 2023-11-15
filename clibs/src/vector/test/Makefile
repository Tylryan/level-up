LIB_NAME=vector
TEST_DIR=test
LIBS=-l$(LIB_NAME) -lctest

all:
	gcc -g *.c -o vector_test -I../../../include -L../../../libs $(LIBS)
	echo "TESTING"
	./vector_test
