/*
Implementation of graph class test fixtures.
*/

#include <iostream>
#include <string>
#include <stdexcept>
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

    directed_int.set_edge_weight(-1, 0, 1);
    directed_int.set_edge_weight(-1, 7, -1);
    directed_int.set_edge_weight(0, -1, 0);
    CPPUNIT_ASSERT( directed_int.has_edge(-1, 0) );
    CPPUNIT_ASSERT( directed_int.has_edge(-1, 7) );
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 0) == 1 );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(-1, 7) == -1 );
    // Only one edge weight is set with directed graphs.
    CPPUNIT_ASSERT_THROW( directed_int.get_edge_weight(7, -1), std::invalid_argument );
    CPPUNIT_ASSERT( directed_int.get_edge_weight(0, -1) == 0 );

    // Removing int edges one at a time.

    directed_int.remove_edge(-1, 0);
    CPPUNIT_ASSERT( !directed_int.has_edge(-1, 0) );
    // Only one edge is removed with directed graphs.
    CPPUNIT_ASSERT( directed_int.has_edge(0, -1) );

    // Adding boolean edges one at a time.

    directed_string.set_edge_weight("", "Mumbai", false);
    directed_string.set_edge_weight("New York", "Tokyo", true);
    directed_string.set_edge_weight("Mumbai", "", true);
    // Edges with weight `false` should still exist.
    CPPUNIT_ASSERT( directed_string.has_edge("", "Mumbai") );
    CPPUNIT_ASSERT( directed_string.has_edge("Mumbai", "") );
    // has_edge should return false and not raise an error when one of
    // the vertices is not in the graph.
    CPPUNIT_ASSERT( !directed_string.has_edge("Tokyo", "") );

    // Adding float edges several at a time.

    // Invalid call: one of the vertices doesn't exist.
    UserDefinedObject non_vertex = UserDefinedObject(100, 100);
    UserDefinedObject non_vertex_edges[4][2] =
        { {object_vals[0], object_vals[1]},
          {object_vals[0], object_vals[2]},
          {object_vals[1], object_vals[0]},
          {non_vertex, object_vals[0]} };
    float non_vertex_edge_weights[4] = { 0.0f, 0.1f, -1.0f, -1.0f };
    CPPUNIT_ASSERT_THROW( directed_object.set_edge_weights(non_vertex_edges, non_vertex_edge_weights, 4), std::invalid_argument );
    // No edges were added.
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[2]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[1], object_vals[0]) );

    UserDefinedObject edges[3][2] = { {object_vals[0], object_vals[1]},
                                      {object_vals[0], object_vals[2]},
                                      {object_vals[1], object_vals[0]} };
    float edge_weights[3] = { 0.0f, -0.1f, 1.0f };
    directed_object.set_edge_weights(edges, edge_weights, 3);
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
    UserDefinedObject invalid_removal_edges[2][2] =
        { {object_vals[0], object_vals[1]},
          {object_vals[0], non_vertex} };
    CPPUNIT_ASSERT_THROW( directed_object.remove_edges(invalid_removal_edges, 2),
        std::invalid_argument );
    // No edges were removed.
    CPPUNIT_ASSERT( directed_object.has_edge(object_vals[0], object_vals[1]) );

    UserDefinedObject removal_edges[2][2] = { {object_vals[0], object_vals[1]},
                                              {object_vals[0], object_vals[2]} };
    directed_object.remove_edges(removal_edges, 2);
    // All edges were removed.
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[1]) );
    CPPUNIT_ASSERT( !directed_object.has_edge(object_vals[0], object_vals[2]) );
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