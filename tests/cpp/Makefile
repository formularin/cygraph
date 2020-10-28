CC := g++
STD := -std=c++14

all: setup bin/test_cygraph

setup:
	mkdir -p build bin

bin/test_cygraph: build/utils.o build/test_algorithms.o build/test_graph.o script/test_cygraph.cpp
	$(CC) $(STD) script/test_cygraph.cpp build/*.o -o bin/test_cygraph

build/test_algorithms.o: src/test_algorithms.cpp build/utils.o
	$(CC) $(STD) -c src/test_algorithms.cpp build/utils.o -o build/test_algorithms.o

build/test_graph.o: src/test_graph.cpp build/catch.o build/utils.o
	$(CC) $(STD) -c src/test_graph.cpp build/utils.o -o build/test_graph.o

build/utils.o: include/utils.hpp
	$(CC) $(STD) -c include/utils.hpp -o build/utils.o

clean:
	rm -rf build bin