/*
Test fixtures required for testing algorithm functions.
*/

#ifndef TEST_ALGORITHMS_HPP
#define TEST_ALGORITHMS_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

#include "../../../include/cygraph.hpp"
#include "../include/utils.hpp"


using namespace cygraph;
using cygraph_utils::UserDefinedObject;


class TestArticulationPoints: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_get_articulation_points();

        static CppUnit::Test* suite();
};


class TestComponents: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_get_connected_components();
        void test_get_number_connected_components();
        void test_get_strongly_connected_components();
        void test_get_number_strongly_connected_components();

        static CppUnit::Test* suite();
};


class TestPartitioning: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_partition_karger();

        static CppUnit::Test* suite();
};


class TestShortestPath: public CppUnit::TestFixture {
    public:
        void setUp();
        void tearDown();

        void test_get_shortest_path_dijkstra();

        static CppUnit::Test* suite();
};

#endif