
BIN=ctest

archive: objects
	ar rcsv lib$(BIN).a $(BIN).o
	cp $(BIN).h ../../include
	cp lib$(BIN).a ../../libs

objects: $(BIN).c
	gcc -c $^

example:
	gcc example.c ctest.c -o example
	./example
clean:
	rm *.o
	rm ../../include/$(BIN).h
	rm ../../libs/lib$(BIN).a
