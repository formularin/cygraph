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


class TestUnweightedAdjacencyListGraph: public CppUnit::TestFixture {

    private:
        UnweightedAdjacencyListGraph<int> directed_int;
        UnweightedAdjacencyListGraph<std::string> directed_string;

        UnweightedAdjacencyListGraph<int> undirected_int;
        UnweightedAdjacencyListGraph<std::string> undirected_string;

    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};


class TestWeightedAdjacencyListGraph: public CppUnit::TestFixture {

    private:
        WeightedAdjacencyListGraph<int, int> directed_int;
        WeightedAdjacencyListGraph<UserDefinedObject, float> directed_object;

        WeightedAdjacencyListGraph<int, int> undirected_int;
        WeightedAdjacencyListGraph<UserDefinedObject, float> undirected_object;

    public:
        void setUp();
        void tearDown();

        void test_edges();
        void test_family();
        void test_vertices();

        static CppUnit::Test* suite();
};

#endif