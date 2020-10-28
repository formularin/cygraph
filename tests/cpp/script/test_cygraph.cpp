/*
Runs all C++ Cygraph unit tests.
*/

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "../include/test_algorithms.hpp"
#include "../include/test_graph.hpp"


int main( int argc, char** argv ) {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = 
        CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    runner.run();
}