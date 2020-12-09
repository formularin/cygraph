/*
Implementation of adjacency list class test fixtures.
*/

#include <cppunit/TestCaller.h>

#include "../include/test_adjacency_list.hpp"
#include "../../../include/graph/adjacency_list.hpp"


// --------- UNWEIGHTED ---------


void TestUnweightedAdjacencyListGraph::setUp() {
    /*
    Creates UnweightedAdjacencyListGraph objects with varying vertex and edge
    weight types.
    */

    directed_int = UnweightedAdjacencyListGraph<int>(
        true, unordered_set<int>(int_vals.begin(), int_vals.end()));
    directed_string = UnweightedAdjacencyListGraph<std::string>(
        true, unordered_set<std::string>(string_vals.begin(), string_vals.end()));

    undirected_int = UnweightedAdjacencyListGraph<int>(
        false, unordered_set<int>(int_vals.begin(), int_vals.end()));
    undirected_string = UnweightedAdjacencyListGraph<std::string>(
        false, unordered_set<std::string>(string_vals.begin(), string_vals.end()));
}


void TestUnweightedAdjacencyListGraph::tearDown() {}


void TestUnweightedAdjacencyListGraph::test_edges() {
    /*
    Tests the following methods:
        - UnweightedAdjacencyListGraph::set_edge_weight
        - UnweightedAdjacencyListGraph::set_edge_weights
        - UnweightedAdjacencyListGraph::remove_edge
        - UnweightedAdjacencyListGraph::remove_edges
        - UnweightedAdjacencyListGraph::has_edge
        - UnweightedAdjacencyListGraph::get_edge_weight
    */

    // DIRECTED GRAPHS

    // Adding edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.add_edge(-1, 0) );
    // Add edge using set_edge_weight
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 7, true) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.add_edge(0, -1) );
    CPPUNIT_ASSERT( directed_int.has_edge(-1, 0) );
    // Check edge existence with get_edge_weight
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 7) );
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );
    // Only one edge is added with directed graphs.
    CPPUNIT_ASSERT( !directed_int.has_edge(7, -1) );

    // Removing edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.remove_edge(-1, 0) );
    // Remove edge using set_edge_weight
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 7, false) );
    CPPUNIT_ASSERT( !directed_int.has_edge(-1, 0) );
    // Only one edge is removed with directed graphs.
    CPPUNIT_ASSERT( directed_int.get_edge_weight(0, -1) );

    // Adding edges several at a time.

    // Invalid call: one of the vertices doesn't exist.
    std::string non_vertex = "Beijing";
    vector<pair<std::string, std::string>> edges = {
        { string_vals[0], string_vals[1] },
        { string_vals[0], string_vals[2] },
        { string_vals[1], string_vals[0] },
        { non_vertex, string_vals[0] }
    };
    CPPUNIT_ASSERT_THROW( directed_string.add_edges(edges), std::invalid_argument );
    // No edges were added.
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[2]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[1], string_vals[0]) );

    edges = {
        { string_vals[0], string_vals[1] },
        { string_vals[0], string_vals[2] },
        { string_vals[1], string_vals[0] },
    };
    CPPUNIT_ASSERT_NO_THROW( directed_string.add_edges(edges) );
    // All edges were added.
    CPPUNIT_ASSERT( directed_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( directed_string.has_edge(string_vals[0], string_vals[2]) );
    CPPUNIT_ASSERT( directed_string.get_edge_weight(string_vals[1], string_vals[0]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[2], string_vals[0]) );
    CPPUNIT_ASSERT_THROW( directed_string.get_edge_weight(string_vals[2], string_vals[0]),
        std::invalid_argument );

    // Removing edges several at a time.

    // Invalid call: one of the vertices doesn't exist.
    vector<pair<std::string, std::string>> removal_edges = {
        {string_vals[0], string_vals[1]},
        {string_vals[0], non_vertex}
    };
    CPPUNIT_ASSERT_THROW( directed_string.remove_edges(removal_edges), std::invalid_argument );
    // No edges were removed.
    CPPUNIT_ASSERT( directed_string.has_edge(string_vals[0], string_vals[1]) );

    removal_edges = {
        {string_vals[0], string_vals[1]},
        {string_vals[0], string_vals[2]}
    };
    CPPUNIT_ASSERT_NO_THROW( directed_string.remove_edges(removal_edges) );
    // All edges were removed.
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[2]) );
}


void TestUnweightedAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - UnweightedAdjacencyListGraph::get_children
        - UnweightedAdjacencyListGraph::get_parents
    */
}


void TestUnweightedAdjacencyListGraph::test_vertices() {
    /*
    Tests the following methods:
        - UnweightedAdjacencyListGraph::add_vertex
        - UnweightedAdjacencyListGraph::add_vertices
        - UnweightedAdjacencyListGraph::remove_vertex
        - UnweightedAdjacencyListGraph::remove_vertices
        - UnweightedAdjacencyListGraph::has_vertex
        - UnweightedAdjacencyListGraph::get_vertices
    */
}


CppUnit::Test* TestUnweightedAdjacencyListGraph::suite() {

    CppUnit::TestSuite* test_suite =
        new CppUnit::TestSuite("TestUnweightedAdjacencyListGraph");
    test_suite->addTest(
        new CppUnit::TestCaller<TestUnweightedAdjacencyListGraph>(
            "test_edges",
            &TestUnweightedAdjacencyListGraph::test_edges
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestUnweightedAdjacencyListGraph>(
            "test_vertices",
            &TestUnweightedAdjacencyListGraph::test_vertices
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestUnweightedAdjacencyListGraph>(
            "test_family",
            &TestUnweightedAdjacencyListGraph::test_family
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