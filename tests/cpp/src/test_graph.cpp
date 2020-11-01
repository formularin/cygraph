/*
Implementation of graph class test fixtures.
*/

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <cppunit/TestCaller.h>

#include "../include/utils.hpp"
#include "../include/test_graph.hpp"
#include "../../../include/graph/adjacency_list.hpp"
#include "../../../include/graph/adjacency_matrix.hpp"


using namespace cygraph;
using cygraph_utils::UserDefinedObject;


// ADJACENCY MATRIX
// ================


void TestAdjacencyMatrixGraph::setUp() {
    /*
    Creates AdjacencyMatrixGraph objects with varying vertex and edge
    weight types.
    */
}


void TestAdjacencyMatrixGraph::tearDown() {}


void TestAdjacencyMatrixGraph::test_edges() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::set_edge_weight
        - AdjacencyMatrixGraph::set_edge_weights
        - AdjacencyMatrixGraph::remove_edge
        - AdjacencyMatrixGraph::has_edge
        - AdjacencyMatrixGraph::get_edge_weight
    */
}


void TestAdjacencyMatrixGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::get_children
        - AdjacencyMatrixGraph::get_parents
    */
}


void TestAdjacencyMatrixGraph::test_vertices() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::add_vertex
        - AdjacencyMatrixGraph::add_vertices
        - AdjacencyMatrixGraph::remove_vertex
        - AdjacencyMatrixGraph::has_vertex
        - AdjacencyMatrixGraph::get_vertices
    */
}


CppUnit::Test* TestAdjacencyMatrixGraph::suite() {

    CppUnit::TestSuite* test_suite =
        new CppUnit::TestSuite("TestAdjacencyMatrixGraph");
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyMatrixGraph>(
            "test_edges",
            &TestAdjacencyMatrixGraph::test_edges
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyMatrixGraph>(
            "test_vertices",
            &TestAdjacencyMatrixGraph::test_vertices
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyMatrixGraph>(
            "test_family",
            &TestAdjacencyMatrixGraph::test_family
        )
    );
    return test_suite;
}


// ADJACENCY LIST
// ================


void TestAdjacencyListGraph::setUp() {
    /*
    Creates AdjacencyListGraph objects with varying vertex and edge
    weight types.
    */
}


void TestAdjacencyListGraph::tearDown() {}


void TestAdjacencyListGraph::test_edges() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::set_edge_weight
        - AdjacencyMatrixGraph::set_edge_weights
        - AdjacencyMatrixGraph::remove_edge
        - AdjacencyMatrixGraph::has_edge
        - AdjacencyMatrixGraph::get_edge_weight
    */
}


void TestAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::get_children
        - AdjacencyMatrixGraph::get_parents
    */
}


void TestAdjacencyListGraph::test_vertices() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::add_vertex
        - AdjacencyMatrixGraph::add_vertices
        - AdjacencyMatrixGraph::remove_vertex
        - AdjacencyMatrixGraph::has_vertex
        - AdjacencyMatrixGraph::get_vertices
    */
}


CppUnit::Test* TestAdjacencyListGraph::suite() {

    CppUnit::TestSuite* test_suite =
        new CppUnit::TestSuite("TestAdjacencyListGraph");
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyListGraph>(
            "test_edges",
            &TestAdjacencyListGraph::test_edges
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyListGraph>(
            "test_vertices",
            &TestAdjacencyListGraph::test_vertices
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestAdjacencyListGraph>(
            "test_family",
            &TestAdjacencyListGraph::test_family
        )
    );
    return test_suite;
}