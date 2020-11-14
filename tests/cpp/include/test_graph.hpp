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


// Sample values.
const std::vector<int> int_vals = {-1, 0, 1, 7};
const std::vector<float> float_vals = {-1.0f, -0.5f, 0.0f, 0.5f};
const std::vector<std::string> string_vals = {"", "Mumbai", "New York", "Tokyo"};
const std::vector<bool> bool_vals = {true, false, true, false};
const std::vector<UserDefinedObject> object_vals = {
    UserDefinedObject(), UserDefinedObject(-2, 0),
    UserDefinedObject(1, 1), UserDefinedObject(1, 2)
};


class TestAdjacencyMatrixGraph: public CppUnit::TestFixture {

    private:
        AdjacencyMatrixGraph<int, int> directed_int;
        AdjacencyMatrixGraph<std::string, bool> directed_string;
        AdjacencyMatrixGraph<UserDefinedObject, float> directed_object;

        AdjacencyMatrixGraph<int, int> undirected_int;
        AdjacencyMatrixGraph<std::string, bool> undirected_string;
        AdjacencyMatrixGraph<UserDefinedObject, float> undirected_object;

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