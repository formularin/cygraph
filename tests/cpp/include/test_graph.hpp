/*
Test suites required for testing the graph classes.
*/

#ifndef TEST_GRAPH_HPP
#define TEST_GRAPH_HPP

#include <string>
#include <tuple>
#include <vector>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include "../../../include/cygraph.hpp"
#include "../include/utils.hpp"


using namespace cygraph;
using cygraph_utils::UserDefinedObject;


class TestAdjacencyMatrixGraph: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};


class TestAdjacencyListGraph: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};

#endif