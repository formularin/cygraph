/*
Implementation of adjacency list class test fixtures.
*/


#include "../include/test_adjacency_list.hpp"
#include "../../../include/graph/adjacency_list.hpp"


// --------- UNWEIGHTED ---------


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
        - AdjacencyListGraph::set_edge_weight
        - AdjacencyListGraph::set_edge_weights
        - AdjacencyListGraph::remove_edge
        - AdjacencyListGraph::remove_edges
        - AdjacencyListGraph::has_edge
        - AdjacencyListGraph::get_edge_weight
    */
}


void TestAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyListGraph::get_children
        - AdjacencyListGraph::get_parents
    */
}


void TestAdjacencyListGraph::test_vertices() {
    /*
    Tests the following methods:
        - AdjacencyListGraph::add_vertex
        - AdjacencyListGraph::add_vertices
        - AdjacencyListGraph::remove_vertex
        - AdjacencyListGraph::remove_vertices
        - AdjacencyListGraph::has_vertex
        - AdjacencyListGraph::get_vertices
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



// --------- WEIGHTED ---------


void TestWeightedAdjacencyListGraph::setUp() {
    /*
    Creates WeightedAdjacencyListGraph objects with varying vertex and edge
    weight types.
    */
}


void TestWeightedAdjacencyListGraph::tearDown() {}


void TestWeightedAdjacencyListGraph::test_edges() {
    /*
    Tests the following methods:
        - WeightedAdjacencyListGraph::set_edge_weight
        - WeightedAdjacencyListGraph::set_edge_weights
        - WeightedAdjacencyListGraph::remove_edge
        - WeightedAdjacencyListGraph::remove_edges
        - WeightedAdjacencyListGraph::has_edge
        - WeightedAdjacencyListGraph::get_edge_weight
    */
}


void TestWeightedAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - WeightedAdjacencyListGraph::get_children
        - WeightedAdjacencyListGraph::get_parents
    */
}


void TestWeightedAdjacencyListGraph::test_vertices() {
    /*
    Tests the following methods:
        - WeightedAdjacencyListGraph::add_vertex
        - WeightedAdjacencyListGraph::add_vertices
        - WeightedAdjacencyListGraph::remove_vertex
        - WeightedAdjacencyListGraph::remove_vertices
        - WeightedAdjacencyListGraph::has_vertex
        - WeightedAdjacencyListGraph::get_vertices
    */
}


CppUnit::Test* TestWeightedAdjacencyListGraph::suite() {

    CppUnit::TestSuite* test_suite =
        new CppUnit::TestSuite("TestAdjacencyListGraph");
    test_suite->addTest(
        new CppUnit::TestCaller<TestWeightedAdjacencyListGraph>(
            "test_edges",
            &TestWeightedAdjacencyListGraph::test_edges
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestWeightedAdjacencyListGraph>(
            "test_vertices",
            &TestWeightedAdjacencyListGraph::test_vertices
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestWeightedAdjacencyListGraph>(
            "test_family",
            &TestWeightedAdjacencyListGraph::test_family
        )
    );
    return test_suite;
}