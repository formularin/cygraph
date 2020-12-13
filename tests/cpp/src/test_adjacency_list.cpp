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
    CPPUNIT_ASSERT( !directed_string.get_edge_weight(string_vals[2], string_vals[0]) );

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

    // UNDIRECTED GRAPHS

    // Adding edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.add_edge(-1, 0) );
    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weight(-1, 7, true) );
    CPPUNIT_ASSERT( undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( undirected_int.has_edge(0, -1) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(0, -1) );

    // Removing edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.remove_edge(-1, 0) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( !undirected_int.get_edge_weight(0, -1) );
    // Removing an edge that doesn't exist.
    CPPUNIT_ASSERT_THROW( undirected_int.remove_edge(-1, 0), std::invalid_argument );

    // Adding edges several at a time.
    vector<tuple<int, int, bool>> edges_with_weights = {
        { -1, 7, false },
        { 1, 0, true },
        { 1, 7, true }
    };
    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weights(edges_with_weights) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(-1, 7) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(7, -1) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(1, 0) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(0, 1) );
    CPPUNIT_ASSERT( undirected_int.has_edge(1, 7) );
    CPPUNIT_ASSERT( undirected_int.has_edge(1, 7) );

    // Changing edge weights.
    CPPUNIT_ASSERT_NO_THROW( undirected_string.set_edge_weight("Mumbai", "New York", true) );
    CPPUNIT_ASSERT( undirected_string.has_edge("Mumbai", "New York") );
    CPPUNIT_ASSERT( undirected_string.has_edge("New York", "Mumbai") );

    // has_edge returns false when one of the vertices doesn't exist.
    CPPUNIT_ASSERT( !directed_int.has_edge(420, 69) );
}


void TestUnweightedAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - UnweightedAdjacencyListGraph::get_children
        - UnweightedAdjacencyListGraph::get_parents
    */

    // DIRECTED GRAPHS

    CPPUNIT_ASSERT_NO_THROW( directed_int.add_edge(-1, 0) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.add_edge(-1, 1) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.add_edge(-1, 7) );

    // get_children

    CPPUNIT_ASSERT( (directed_int.get_children(-1) == (std::unordered_set<int>) {0, 1, 7}) );
    CPPUNIT_ASSERT( directed_int.get_children(0) == std::unordered_set<int>() );
    CPPUNIT_ASSERT_THROW( directed_int.get_children(10), std::invalid_argument );


    // get_parents

    CPPUNIT_ASSERT( (directed_int.get_parents(0) == std::unordered_set<int> {-1}) );
    CPPUNIT_ASSERT( directed_int.get_parents(-1) == std::unordered_set<int>() );

    // UNDIRECTED GRAPHS

    CPPUNIT_ASSERT_NO_THROW( undirected_string.add_edge("Mumbai", "New York") );
    CPPUNIT_ASSERT_NO_THROW( undirected_string.add_edge("Mumbai", "Tokyo") );
    CPPUNIT_ASSERT_NO_THROW( undirected_string.add_edge("Mumbai", "Mumbai") );

    std::unordered_set<std::string> mumbai_neighbors = {"Mumbai", "New York", "Tokyo"};
    std::unordered_set<std::string> new_york_neighbors = {"Mumbai"};

    // get_children

    CPPUNIT_ASSERT( (undirected_string.get_children("Mumbai") == mumbai_neighbors) );
    CPPUNIT_ASSERT( (undirected_string.get_children("New York") == new_york_neighbors) );

    // get_parents

    CPPUNIT_ASSERT( (undirected_string.get_parents("Mumbai") == mumbai_neighbors) );
    CPPUNIT_ASSERT( (undirected_string.get_parents("New York") == new_york_neighbors) );
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