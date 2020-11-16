/*
Implementation of graph class test fixtures.
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <cppunit/TestCaller.h>

#include "../include/utils.hpp"
#include "../include/test_graph.hpp"
#include "../../../include/graph/adjacency_list.hpp"
#include "../../../include/graph/adjacency_matrix.hpp"


using std::pair;
using std::tuple;
using std::unordered_set;

using namespace cygraph;
using cygraph_utils::UserDefinedObject;


// ADJACENCY MATRIX
// ================


void TestAdjacencyMatrixGraph::setUp() {
    /*
    Creates AdjacencyMatrixGraph objects with varying vertex and edge
    weight types.
    */

    directed_int = AdjacencyMatrixGraph<int, int>(true, int_vals);
    directed_string = AdjacencyMatrixGraph<std::string, bool>(true, string_vals);
    directed_object = AdjacencyMatrixGraph<UserDefinedObject, float>(true, object_vals);

    undirected_int = AdjacencyMatrixGraph<int, int>(false, int_vals);
    undirected_string = AdjacencyMatrixGraph<std::string, bool>(false, string_vals);
    undirected_object = AdjacencyMatrixGraph<UserDefinedObject, float>(false, object_vals);
}


void TestAdjacencyMatrixGraph::tearDown() {}


void TestAdjacencyMatrixGraph::test_edges() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::set_edge_weight
        - AdjacencyMatrixGraph::set_edge_weights
        - AdjacencyMatrixGraph::remove_edge
        - AdjacencyMatrixGraph::remove_edges
        - AdjacencyMatrixGraph::has_edge
        - AdjacencyMatrixGraph::get_edge_weight
    */

    // DIRECTED GRAPHS

    // Adding int edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 0, 1) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(-1, 7, -1) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(0, -1, 0) );
    CPPUNIT_ASSERT( directed_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( directed_int.has_edge(-1, 7) );
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 0) == 1 );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 7) == -1 );
    // Only one edge weight is set with directed graphs.
    CPPUNIT_ASSERT_THROW( directed_int.get_edge_weight(7, -1), std::invalid_argument );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(0, -1) == 0 );

    // Removing int edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.remove_edge(-1, 0) );
    CPPUNIT_ASSERT( !directed_int.has_edge(-1, 0) );
    // Only one edge is removed with directed graphs.
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );

    // Adding boolean edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_string.set_edge_weight("", "Mumbai", false) );
    CPPUNIT_ASSERT_NO_THROW( directed_string.set_edge_weight("New York", "Tokyo", true) );
    CPPUNIT_ASSERT_NO_THROW( directed_string.set_edge_weight("Mumbai", "", true) );
    // Edges with weight `false` should still exist.
    CPPUNIT_ASSERT( directed_string.has_edge("", "Mumbai") );
    CPPUNIT_ASSERT( directed_string.has_edge("Mumbai", "") );

    // Adding float edges several at a time.

    // Invalid call: one of the vertices doesn't exist.
    UserDefinedObject non_vertex = UserDefinedObject(100, 100);
    vector<tuple<UserDefinedObject, UserDefinedObject, float>> edges = {
        { object_vals[0], object_vals[1], 0.0f },
        { object_vals[0], object_vals[2], 0.1f },
        { object_vals[1], object_vals[0], -1.0f },
        { non_vertex, object_vals[0], -1.0f }
    };
    CPPUNIT_ASSERT_THROW( directed_object.set_edge_weights(edges), std::invalid_argument );
    // No edges were added.
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[2]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[1], object_vals[0]) );

    edges = {
        { object_vals[0], object_vals[1], 0.0f },
        { object_vals[0], object_vals[2], -1.0f },
        { object_vals[1], object_vals[0], 1.0f }
    };
    CPPUNIT_ASSERT_NO_THROW( directed_object.set_edge_weights(edges) );
    // All edges were added.
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[0], object_vals[2]) );
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[1], object_vals[0]) );
    CPPUNIT_ASSERT( directed_object.get_edge_weight(object_vals[0], object_vals[1]) == 0.0f );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[2], object_vals[0]) );
    CPPUNIT_ASSERT_THROW( directed_object.get_edge_weight(object_vals[2], object_vals[0]),
        std::invalid_argument );

    // Removing float edges several at a time.

    // Invalid call: one of the vertices doesn't exist.
    vector<pair<UserDefinedObject, UserDefinedObject>> invalid_removal_edges = {
        {object_vals[0], object_vals[1]},
        {object_vals[0], non_vertex}
    };
    CPPUNIT_ASSERT_THROW( directed_object.remove_edges(invalid_removal_edges),
                          std::invalid_argument );
    // No edges were removed.
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[0], object_vals[1]) );

    vector<pair<UserDefinedObject, UserDefinedObject>> removal_edges = {
        {object_vals[0], object_vals[1]},
        {object_vals[0], object_vals[2]}
    };
    CPPUNIT_ASSERT_NO_THROW( directed_object.remove_edges(removal_edges) );
    // All edges were removed.
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[2]) );

    // UNDIRECTED GRAPHS

    // Adding int edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weight(-1, 0, 1) );
    CPPUNIT_ASSERT_NO_THROW( undirected_int.set_edge_weight(-1, 7, 0) );
    CPPUNIT_ASSERT( undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( undirected_int.has_edge(0, -1) );
    CPPUNIT_ASSERT( undirected_int.has_edge(-1, 7) );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(-1, 0) == 1 );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(0, -1) == 1 );
    CPPUNIT_ASSERT( undirected_int.get_edge_weight(-1, 7) == 0 );

    // Removing int edges one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_int.remove_edge(-1, 0) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( !undirected_int.has_edge(0, -1) );
    CPPUNIT_ASSERT_THROW( undirected_int.get_edge_weight(-1, 0), std::invalid_argument );
    // Removing an edge that doesn't exist should raise a warning, not throw an error.
    CPPUNIT_ASSERT_NO_THROW( undirected_int.remove_edge(-1, 0) );

    // Adding float edges several at a time.
    vector<tuple<UserDefinedObject, UserDefinedObject, float>> undirected_edges = {
        { object_vals[0], object_vals[1], 0.0f },
        { object_vals[0], object_vals[2], -1.0f },
        { object_vals[1], object_vals[2], 0.5f }
    };
    CPPUNIT_ASSERT_NO_THROW( undirected_object.set_edge_weights(undirected_edges) );
    // All edges were added.
    CPPUNIT_ASSERT( undirected_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( undirected_object.has_edge(object_vals[1], object_vals[0]) );
    CPPUNIT_ASSERT( undirected_object.has_edge(object_vals[0], object_vals[2]) );
    CPPUNIT_ASSERT( undirected_object.has_edge(object_vals[1], object_vals[2]) );

    // Changing edge weight values.
    undirected_object.set_edge_weight(object_vals[0], object_vals[1], 3.0f);
    CPPUNIT_ASSERT( undirected_object.get_edge_weight(object_vals[0], object_vals[1]) == 3.0f );
    CPPUNIT_ASSERT( undirected_object.get_edge_weight(object_vals[1], object_vals[0]) == 3.0f );
}


void TestAdjacencyMatrixGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::get_children
        - AdjacencyMatrixGraph::get_parents
    */

    // DIRECTED GRAPHS

    directed_int.set_edge_weight(-1, 0, 1);
    directed_int.set_edge_weight(-1, 1, 1);
    directed_int.set_edge_weight(-1, 7, 1);

    // get_children

    CPPUNIT_ASSERT( (directed_int.get_children(-1) == (std::unordered_set<int>) {0, 1, 7}) );
    CPPUNIT_ASSERT( directed_int.get_children(0) == std::unordered_set<int>() );
    CPPUNIT_ASSERT_THROW( directed_int.get_children(10), std::invalid_argument );

    // get_parents

    CPPUNIT_ASSERT( (directed_int.get_parents(0) == std::unordered_set<int> {-1}) );
    CPPUNIT_ASSERT( directed_int.get_parents(-1) == std::unordered_set<int>() );

    // UNDIRECTED GRAPHS

    undirected_string.set_edge_weight("Mumbai", "New York", true);
    undirected_string.set_edge_weight("Mumbai", "Tokyo", true);
    undirected_string.set_edge_weight("Mumbai", "Mumbai", true);

    std::unordered_set<std::string> mumbai_neighbors = {"Mumbai", "New York", "Tokyo"};
    std::unordered_set<std::string> new_york_neighbors = {"Mumbai"};

    // get_children

    CPPUNIT_ASSERT( (undirected_string.get_children("Mumbai") == mumbai_neighbors) );
    CPPUNIT_ASSERT( (undirected_string.get_children("New York") == new_york_neighbors) );

    // get_parents

    CPPUNIT_ASSERT( (undirected_string.get_parents("Mumbai") == mumbai_neighbors) );
    CPPUNIT_ASSERT( (undirected_string.get_parents("New York") == new_york_neighbors) );
}


void TestAdjacencyMatrixGraph::test_vertices() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::add_vertex
        - AdjacencyMatrixGraph::add_vertices
        - AdjacencyMatrixGraph::remove_vertex
        - AdjacencyMatrixGraph::remove_vertices
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