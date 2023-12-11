

all: a.out

a.out: main.o io.o lexer.o parser.o generator.o writer.o
	g++ -g $^ -o $@

main.o: main.cpp io.cpp lexer.cpp parser.cpp generator.cpp writer.cpp
	g++ -g $^ -c

clean:
	rm *.o
