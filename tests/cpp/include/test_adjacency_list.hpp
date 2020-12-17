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

    private:
        AdjacencyListGraph<int, int> directed_int;
        AdjacencyListGraph<std::string, float> directed_string;
        AdjacencyListGraph<UserDefinedObject, bool> directed_object;

        AdjacencyListGraph<int, int> undirected_int;
        AdjacencyListGraph<std::string, bool> undirected_string;
        AdjacencyListGraph<UserDefinedObject, float> undirected_object;

    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};

#endif