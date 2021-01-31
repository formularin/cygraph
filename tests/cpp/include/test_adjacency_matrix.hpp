/* ============================================================================================== *
 * test_adjacency_matrix.hpp                                                                      *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * CppUnit classes for testing adjacency-matrix-implemented graph classes.                        *
 * ============================================================================================== */


#ifndef TEST_ADJACENCY_MATRIX_HPP
#define TEST_ADJACENCY_MATRIX_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include "../../../include/cygraph.hpp"
#include "../include/utils.hpp"


using namespace cygraph;


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

#endif