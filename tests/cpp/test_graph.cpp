/*
Tests the Graph classes.
*/

#include <any>
#include <unordered_map>
#include <vector>

#include "cygraph.hpp"

using std::any;


int main() {
    std::vector<int> vertices = {1, 2, 3};
    cygraph::Graph<int, int> graph(false, &vertices);
}