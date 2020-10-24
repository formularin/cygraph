COMPILER := g++
OPTIONS := -std=c++17

all: setup bin/test_algorithms bin/test_graph

setup:
	mkdir -p build bin

bin/test_algorithms: script/test_algorithms.cpp build/catch.o build/utils.o
	$(COMPILER) $(OPTIONS) script/test_algorithms.cpp -L build/ -o bin/test_algorithms

bin/test_graph: script/test_graph.cpp build/catch.o build/utils.o
	$(COMPILER) $(OPTIONS) script/test_graph.cpp -L build/ -o bin/test_graph

build/catch.o: lib/catch.cpp
	$(COMPILER) $(OPTIONS) -c lib/catch.cpp -o build/catch.o

build/utils.o: include/utils.hpp
	$(COMPILER) $(OPTIONS) -c include/utils.hpp -o build/utils.o

clean:
	rm -rf build bin