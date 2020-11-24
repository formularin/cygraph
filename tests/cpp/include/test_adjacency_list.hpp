/*
Tests suites for testing WeightedAdajcencyListGraph and AdjacencyListGraph.
*/


#ifndef TEST_ADJACENCY_LIST_HPP
#define TEST_ADJACENCY_LIST_HPP


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include "../../../include/cygraph.hpp"
#include "../include/utils.hpp"


using namespace cygraph;


class TestAdjacencyListGraph: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};


class TestWeightedAdjacencyListGraph: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};

#endif