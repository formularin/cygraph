COMPILER := g++
OPTIONS := -std=c++17 -I ../../include/
OBJECTS := build/test_algorithms.o build/test_graph.o

all: setup $(OBJECTS)
	$(COMPILER) $(OPTIONS) $(OBJECTS) -o bin/test_cygraph

setup:
	mkdir -p build bin

build/test_algorithms.o: test_algorithms.cpp
	$(COMPILER) $(OPTIONS) -c test_algorithms.cpp -o build/test_algorithms.o

build/test_graph.o: test_graph.cpp
	$(COMPILER) $(OPTIONS) -c test_graph.cpp -o build/test_graph.o

clean:
	rm -rf build/
	rm -rf bin/