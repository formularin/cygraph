COMPILER := g++
OPTIONS := -std=c++14

all: setup bin/test_algorithms bin/test_graph

setup:
	mkdir -p build bin

bin/test_algorithms: script/test_algorithms.cpp build/utils.o
	$(COMPILER) $(OPTIONS) script/test_algorithms.cpp build/*.o -o bin/test_algorithms

bin/test_graph: script/test_graph.cpp build/catch.o build/utils.o
	$(COMPILER) $(OPTIONS) script/test_graph.cpp build/*.o -o bin/test_graph

build/utils.o: include/utils.hpp
	$(COMPILER) $(OPTIONS) -c include/utils.hpp -o build/utils.o

clean:
	rm -rf build bin