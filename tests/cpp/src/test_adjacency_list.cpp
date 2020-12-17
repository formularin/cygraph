/*
Implementation of adjacency list class test fixtures.
*/

#include <cppunit/TestCaller.h>

#include "../include/test_adjacency_list.hpp"
#include "../../../include/graph/adjacency_list.hpp"


void TestAdjacencyListGraph::setUp() {
    /*
    Creates AdjacencyListGraph objects with varying vertex and edge
    weight types.
    */

    directed_int = AdjacencyListGraph<int, int>(
        true, unordered_set<int>(int_vals.begin(), int_vals.end()));
    directed_string = AdjacencyListGraph<std::string, float>(
        true, unordered_set<std::string>(string_vals.begin(), string_vals.end()));
    directed_object = AdjacencyListGraph<UserDefinedObject, bool>(
        true, unordered_set<UserDefinedObject>(object_vals.begin(), object_vals.end());

    undirected_int = AdjacencyListGraph<int, int>(
        false, unordered_set<int>(int_vals.begin(), int_vals.end()));
    undirected_string = AdjacencyListGraph<std::string, bool>(
        false, unordered_set<std::string>(string_vals.begin(), string_vals.end()));
    undirected_object = AdjacencyListGraph<UserDefinedObject, float>(
        false, unordered_set<UserDefinedObject>(object_vals.begin(), object_vals.end()));
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

    // DIRECTED GRAPHS

    // Setting edge weights one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 0, 0) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 7, 200) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(0, -1, -100) );

    CPPUNIT_ASSERT( directed_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 7) == 200 );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(0, -1) == -100 );
    // Only one edge is added with directed graphs.
    CPPUNIT_ASSERT( !directed_int.has_edge(7, -1) );

    // Adding edges one at a time (unweighted).

    CPPUNIT_ASSERT_NO_THROW( directed_object.add_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT_NO_THROW( directed_object.add_edge(object_vals[3], object_vals[2]) );

    CPPUNIT_ASSERT_NO_THROW( directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT_NO_THROW( directed_object.has_edge(object_vals[3], object_vals[2]) );
    CPPUNIT_ASSERT_NO_THROW( directed_object.get_edge_weight(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT_NO_THROW( directed_object.get_edge_weight(object_vals[3], object_vals[2]) );
    CPPUNIT_ASSERT_NO_THROW( !directed_object.has_edge(object_vals[1], object_vals[0]) );
    CPPUNIT_ASSERT_NO_THROW( !directed_object.has_edge(object_vals[2], object_vals[3]) );
    CPPUNIT_ASSERT_NO_THROW( !directed_object.get_edge_weight(object_vals[1], object_vals[0]) );
    CPPUNIT_ASSERT_NO_THROW( !directed_object.get_edge_weight(object_vals[2], object_vals[3]) );

    // Removing edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.remove_edge(-1, 0) );
    CPPUNIT_ASSERT( !directed_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT_THROW( directed_int.get_edge_weight(-1, 0), std::invalid_argument );
    // Only one edge is removed with directed graphs.
    CPPUNIT_ASSERT( directed_int.get_edge_weight(0, -1) == -100 );
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );

    // Removing edges one at a time (unweighted).

    CPPUNIT_ASSERT_NO_THROW( directed_object.remove_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.get_edge_weight(object_vals[0], object_vals[1]) );
    // Only one edge is removed with directed graphs.
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[1], object_vals[0]) );
    CPPUNIT_ASSERT( directed_object.get_edge_weight(object_vals[1], object_vals[0]) );

    // Setting edge weights several at a time.

    // Invalid call: one of the vertices doesn't exist.
    std::string non_vertex = "Beijing";
    vector<tuple<std::string, std::string, float>> edges = {
        { string_vals[0], string_vals[1], 0.0f },
        { string_vals[0], string_vals[2], 1.0f },
        { string_vals[1], string_vals[0], -1.0f },
        { non_vertex, string_vals[0], 100.0f }
    };
    CPPUNIT_ASSERT_THROW( directed_string.set_edge_weights(edges), std::invalid_argument );
    // No edges were added.
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[0], string_vals[2]) );
    CPPUNIT_ASSERT( !directed_string.has_edge(string_vals[1], string_vals[0]) );

    edges = {
        { string_vals[0], string_vals[1], 0.0f },
        { string_vals[0], string_vals[2], 1.0f },
        { string_vals[1], string_vals[0], -1.0f },
    };
    CPPUNIT_ASSERT_NO_THROW( directed_string.set_edge_weights(edges) );
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

    // Setting edge weights one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weight(-1, 0, 0) );
    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weight(-1, 7, -200) );
    CPPUNIT_ASSERT_THROW( undirected_int.set_edge_weight(-200, 7, 1), std::invalid_argument );
    CPPUNIT_ASSERT( undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( undirected_int.has_edge(0, -1) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(0, -1) == 0 );

    // Adding edges one at a time (unweighted).

    CPPUNIT_ASSERT_NO_THROW(
        undirected_string.set_edge_weight(string_vals[0], string_vals[1], true) );
    CPPUNIT_ASSERT_NO_THROW(
        undirected_string.add_edge(string_vals[2], string_vals[3]));
    CPPUNIT_ASSERT_THROW(
        undirected_string.add_edge("Beijing", "New York"), std::invalid_argument );
    CPPUNIT_ASSERT( undirected_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( undirected_string.has_edge(string_vals[1], string_vals[0]) );
    CPPUNIT_ASSERT( undirected_string.get_edge_weight(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( undirected_string.get_edge_weight(string_vals[1], string_vals[0]) );

    // Removing edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.remove_edge(-1, 0) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT_THROW( undirected_int.get_edge_weight(0, -1), std::invalid_argument );
    // Removing an edge that doesn't exist.
    CPPUNIT_ASSERT_THROW( undirected_int.remove_edge(-1, 0), std::invalid_argument );

    // Removing edges one at a time (unweighted)

    CPPUNIT_ASSERT_NO_THROW( undirected_string.remove_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !undirected_string.has_edge(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !undirected_string.has_edge(string_vals[1], string_vals[0]) );
    CPPUNIT_ASSERT( !undirected_string.get_edge_weight(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !undirected_string.get_edge_weight(string_vals[1], string_vals[0]) );
    // Removing an edge that doesn't exist.
    CPPUNIT_ASSERT_THROW(
        undirected_string.remove_edge(string_vals[0], string_vals[3]), std::invalid_argument );

    // Setting edge weights several at a time.
    vector<tuple<int, int, int>> undirected_edges = {
        { -1, 7, 100 },
        { 1, 0, -100 },
        { 1, 7, 0 }
    };
    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weights(undirected_edges) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(-1, 7) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(7, -1) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(1, 0) == -100 );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(0, 1) == -100 );
    CPPUNIT_ASSERT( undirected_int.has_edge(1, 7) );
    CPPUNIT_ASSERT( undirected_int.has_edge(1, 7) );

    // Changing edge weights.
    CPPUNIT_ASSERT_NO_THROW(
        undirected_string.set_edge_weight(string_vals[0], string_vals[1], false) );
    CPPUNIT_ASSERT( !undirected_string.get_edge_weight(string_vals[0], string_vals[1]) );
    CPPUNIT_ASSERT( !undirected_string.get_edge_weight(string_vals[1], string_vals[0]) );

    // has_edge returns false when one of the vertices doesn't exist.
    CPPUNIT_ASSERT( !directed_int.has_edge(420, 69) );
}


void TestAdjacencyListGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyListGraph::get_children
        - AdjacencyListGraph::get_parents
    */

    // DIRECTED GRAPHS

    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 0, 1) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 1, 1) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 7, 1) );

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