COMPILER := g++
STD := -std=c++11
CPPUNIT := -Wl,--no-as-needed -lcppunit

all: setup bin/test_cygraph

setup:
	mkdir -p build bin

bin/test_cygraph: build/test_algorithms.o build/test_adjacency_matrix.o build/test_adjacency_list.o script/test_cygraph.cpp src/utils.cpp ../../src/algorithms.cpp
	$(COMPILER) $(STD) $(CPPUNIT) script/test_cygraph.cpp src/utils.cpp ../../src/algorithms.cpp build/test_*.o -o bin/test_cygraph

build/test_algorithms.o: src/test_algorithms.cpp
	$(COMPILER) $(STD) $(CPPUNIT) -c src/test_algorithms.cpp -o build/test_algorithms.o

build/test_adjacency_list.o: src/test_adjacency_list.cpp
	$(COMPILER) $(STD) $(CPPUNIT) -c src/test_adjacency_list.cpp -o build/test_adjacency_list.o

build/test_adjacency_matrix.o: src/test_adjacency_matrix.cpp
	$(COMPILER) $(STD) $(CPPUNIT) -c src/test_adjacency_matrix.cpp -o build/test_adjacency_matrix.o

clean:
	rm -rf build bin