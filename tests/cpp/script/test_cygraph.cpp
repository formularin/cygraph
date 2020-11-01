/*
Runs all C++ Cygraph unit tests.
*/

#include <cppunit/ui/text/TestRunner.h>

#include "../include/test_algorithms.hpp"
#include "../include/test_graph.hpp"


int main( int argc, char** argv ) {
    CppUnit::TextUi::TestRunner runner;

    // Algorithms
    runner.addTest( TestArticulationPoints::suite() );
    runner.addTest( TestComponents::suite() );
    runner.addTest( TestPartitioning::suite() );
    runner.addTest( TestShortestPath::suite() );

    // Graph classes
    runner.addTest( TestAdjacencyMatrixGraph::suite() );
    runner.addTest( TestAdjacencyListGraph::suite() );

    runner.run();
}