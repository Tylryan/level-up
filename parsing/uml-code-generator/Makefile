
BINARY=u2j

all: $(BINARY)

$(BINARY): main.o io.o lexer.o parser.o generator.o writer.o arg_parser.o 
	g++ -g $^ -o $@

main.o: main.cpp io.cpp lexer.cpp parser.cpp generator.cpp writer.cpp arg_parser.cpp
	g++ -g $^ -c

clean:
	rm *.o

install:
	mv $(BINARY) /usr/local/bin/$(BINARY)
uninstall:
	rm /usr/local/bin/$(BINARY)
