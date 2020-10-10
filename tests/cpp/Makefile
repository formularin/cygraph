COMPILER := g++
OPTIONS := -std=c++17
OBJECTS := build/test_algorithms.o build/test_graph.o

all: $(OBJECTS)
	mkdir -p build
	$(COMPILER) $(OPTIONS) $(OBJECTS) -o test_cygraph

build/test_algorithms.o: test_algorithms.cpp
	$(COMPILER) $(OPTIONS) -c test_algorithms.cpp

build/test_graph.o: test_graph.cpp
	$(COMPILER) $(OPTIONS) -c test_algorithms.cpp

clean:
	rm -rf build/