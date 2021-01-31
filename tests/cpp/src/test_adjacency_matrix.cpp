/*
Implementation of adjacency matrix class test fixture.
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <cppunit/TestCaller.h>

#include "../include/utils.hpp"
#include "../include/test_adjacency_matrix.hpp"
#include "../../../include/cygraph.hpp"


using std::pair;
using std::tuple;
using std::unordered_set;

using namespace cygraph;


void TestAdjacencyMatrixGraph::setUp() {
    /*
    Creates AdjacencyMatrixGraph objects with varying vertex and edge
    weight types.
    */

    directed_int = AdjacencyMatrixGraph<int, int>(
        true, unordered_set<int>(int_vals.begin(), int_vals.end()));
    directed_string = AdjacencyMatrixGraph<std::string, bool>(
        true, unordered_set<std::string>(string_vals.begin(), string_vals.end()));
    directed_object = AdjacencyMatrixGraph<UserDefinedObject, float>(
        true, unordered_set<UserDefinedObject>(object_vals.begin(), object_vals.end()));

    undirected_int = AdjacencyMatrixGraph<int, int>(
        false, unordered_set<int>(int_vals.begin(), int_vals.end()));
    undirected_string = AdjacencyMatrixGraph<std::string, bool>(
        false, unordered_set<std::string>(string_vals.begin(), string_vals.end()));
    undirected_object = AdjacencyMatrixGraph<UserDefinedObject, float>(
        false, unordered_set<UserDefinedObject>(object_vals.begin(), object_vals.end()));
}


void TestAdjacencyMatrixGraph::tearDown() {}


void TestAdjacencyMatrixGraph::test_edges() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::set_edge_weight
        - AdjacencyMatrixGraph::remove_edge
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
    // Removing an edge that doesn't exist.
    CPPUNIT_ASSERT_THROW( undirected_int.remove_edge(-1, 0), std::invalid_argument );

    // Changing edge weight values.
    undirected_object.set_edge_weight(object_vals[0], object_vals[1], 3.0f);
    CPPUNIT_ASSERT( undirected_object.get_edge_weight(object_vals[0], object_vals[1]) == 3.0f );
    CPPUNIT_ASSERT( undirected_object.get_edge_weight(object_vals[1], object_vals[0]) == 3.0f );

    // has_edge returns false when one of the vertices is not in the graph.
    CPPUNIT_ASSERT( !undirected_int.has_edge(420, 69) );
}


void TestAdjacencyMatrixGraph::test_family() {
    /*
    Tests the following methods:
        - AdjacencyMatrixGraph::get_children
        - AdjacencyMatrixGraph::get_parents
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

    CPPUNIT_ASSERT_NO_THROW( undirected_string.set_edge_weight("Mumbai", "New York", true) );
    CPPUNIT_ASSERT_NO_THROW( undirected_string.set_edge_weight("Mumbai", "Tokyo", true) );
    CPPUNIT_ASSERT_NO_THROW( undirected_string.set_edge_weight("Mumbai", "Mumbai", true) );

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
        - AdjacencyMatrixGraph::has_vertex
        - AdjacencyMatrixGraph::get_vertices
    */

    // Adding vertices one at a time.

    CPPUNIT_ASSERT_NO_THROW( directed_int.add_vertex(100) );
    CPPUNIT_ASSERT_NO_THROW( directed_int.add_vertex(1000) );
    CPPUNIT_ASSERT( directed_int.has_vertex(100) );
    CPPUNIT_ASSERT( directed_int.has_vertex(1000) );
    // The vertices can be used to make edges.
    CPPUNIT_ASSERT_NO_THROW( directed_int.set_edge_weight(100, 1000, 1) );
    // Adding vertex that already exists.
    CPPUNIT_ASSERT_THROW( directed_int.add_vertex(100), std::invalid_argument );

    // Removing vertices one at a time.

    CPPUNIT_ASSERT_NO_THROW( undirected_string.remove_vertex("") );
    CPPUNIT_ASSERT_NO_THROW( undirected_string.remove_vertex("New York") );
    CPPUNIT_ASSERT( !undirected_string.has_vertex("") );
    CPPUNIT_ASSERT( !undirected_string.has_vertex("New York") );
    // The vertices can no longer be used to make edges.
    CPPUNIT_ASSERT_THROW( undirected_string.set_edge_weight("", "New York", true),
                          std::invalid_argument );
    // Removing vertex that does not exist.
    CPPUNIT_ASSERT_THROW( undirected_string.remove_vertex("New York"), std::invalid_argument );

    // Adding vertices several at a time.

    // Invalid call: one of the vertices already is in the graph.
    unordered_set<UserDefinedObject> vertices =
        { UserDefinedObject(100, 100), UserDefinedObject(1000, 1000),
          UserDefinedObject(10000, 10000), object_vals[0] };
    CPPUNIT_ASSERT_THROW( directed_object.add_vertices(vertices), std::invalid_argument );
    // No vertices were added.
    CPPUNIT_ASSERT( !directed_object.has_vertex(UserDefinedObject(100, 100)) );
    CPPUNIT_ASSERT( !directed_object.has_vertex(UserDefinedObject(1000, 1000)) );
    CPPUNIT_ASSERT( !directed_object.has_vertex(UserDefinedObject(10000, 10000)) );

    vertices =
        { UserDefinedObject(100, 100), UserDefinedObject(1000, 1000),
          UserDefinedObject(10000, 10000) };
    CPPUNIT_ASSERT_NO_THROW( directed_object.add_vertices(vertices) );
    // All vertices were added.
    CPPUNIT_ASSERT( directed_object.has_vertex(UserDefinedObject(100, 100)) );
    CPPUNIT_ASSERT( directed_object.has_vertex(UserDefinedObject(1000, 1000)) );
    CPPUNIT_ASSERT( directed_object.has_vertex(UserDefinedObject(10000, 10000)) );
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