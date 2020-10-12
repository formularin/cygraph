/*
Tests the Graph classes.
*/

#include <vector>

#include "cygraph.hpp"


int main() {
    std::vector<int> vertices = {1, 2, 3};
    cygraph::AdjacencyMatrixGraph<int, int> graph(false, vertices);
}