/* ============================================================================================== *
 * test_cygraph.cpp                                                                               *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * A program for running all CyGraph unit tests.                                                  *
 * ============================================================================================== */


#include <cppunit/ui/text/TestRunner.h>

#include "../include/test_adjacency_list.hpp"
#include "../include/test_adjacency_matrix.hpp"
#include "../include/test_algorithms.hpp"


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