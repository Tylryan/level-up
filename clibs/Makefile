all:
	make -C ./src/ctest
	make -C ./src/vector
	make -C ./src/logger

install:
	cp ./include/* /usr/local/include
	cp ./libs/* /usr/local/lib

uninstall:
	for include in $(shell ls ./include); do \
		rm /usr/local/include/$$include; \
	done

	for lib in $(shell ls ./libs); do \
		rm /usr/local/lib/$$lib; \
	done

test:
	make -C ./src/vector/test
clean:
	rm libs/*
	rm include/*
