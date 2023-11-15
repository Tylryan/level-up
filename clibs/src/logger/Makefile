

LIB_NAME=logger

all: $(LIB_NAME).h
	cp $(LIB_NAME).h ../../include

example: $(LIB_NAME).h example.c 
	gcc -g example.c -o $@

install: $(LIB_NAME).h
	cp $^ /usr/local/include/$^

uninstall: $(LIB_NAME).h
	rm /usr/local/include/$^
