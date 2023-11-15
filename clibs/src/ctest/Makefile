
BIN=ctest

archive: objects
	ar rcsv lib$(BIN).a $(BIN).o
	cp $(BIN).h ../../include
	cp lib$(BIN).a ../../libs

install:
	cp ../../libs/lib$(BIN).a /usr/local/lib
	cp ../../include/$(BIN).h /usr/local/include

uninstall:
	rm /usr/local/lib/lib$(BIN).a
	rm /usr/local/include/$(BIN).h


objects: $(BIN).c
	gcc -c $^

example:
	gcc example.c ctest.c -o example
	./example
clean:
	rm *.o
	rm ../../include/$(BIN).h
	rm ../../libs/lib$(BIN).a
