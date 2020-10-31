/*
Runs all C++ Cygraph unit tests.
*/

#include <cppunit/ui/text/TestRunner.h>

#include "../include/test_algorithms.hpp"
#include "../include/test_graph.hpp"


int main( int argc, char** argv ) {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( TestArticulationPoints::suite() );
    runner.run();
}