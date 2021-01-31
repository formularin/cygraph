/* ============================================================================================== *
 * test_algorithms.cpp                                                                            *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * Implementations of CppUnit test fixtures for C++ wrappers of graph theory algorithms           *
 * implemented in Cython.                                                                         *
 * ============================================================================================== */


#include <cppunit/TestCaller.h>

#include "../include/test_algorithms.hpp"
#include "../../../include/algorithms.hpp"


// ARTICULATION POINTS
// ===================


void TestArticulationPoints::setUp() {}
void TestArticulationPoints::tearDown() {}
void TestArticulationPoints::test_get_articulation_points() {}


CppUnit::Test* TestArticulationPoints::suite() {

    CppUnit::TestSuite *test_suite =
        new CppUnit::TestSuite("TestArticulationPoints");
    test_suite->addTest(
        new CppUnit::TestCaller<TestArticulationPoints>(
            "test_get_articulation_points",
            &TestArticulationPoints::test_get_articulation_points
        )
    );
    return test_suite;
}


// COMPONENTS
// ==========


void TestComponents::setUp() {}
void TestComponents::tearDown() {}
void TestComponents::test_get_connected_components() {}
void TestComponents::test_get_number_connected_components() {}
void TestComponents::test_get_strongly_connected_components() {}
void TestComponents::test_get_number_strongly_connected_components() {}


CppUnit::Test* TestComponents::suite() {

    CppUnit::TestSuite *test_suite =
        new CppUnit::TestSuite("TestComponents");
    test_suite->addTest(
        new CppUnit::TestCaller<TestComponents>(
            "test_get_connected_components",
            &TestComponents::test_get_connected_components
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestComponents>(
            "test_get_number_connected_components",
            &TestComponents::test_get_number_connected_components
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestComponents>(
            "test_get_strongly_connected_components",
            &TestComponents::test_get_strongly_connected_components
        )
    );
    test_suite->addTest(
        new CppUnit::TestCaller<TestComponents>(
            "test_get_number_strongly_connected_components",
            &TestComponents::test_get_number_strongly_connected_components
        )
    );
    return test_suite;
}


// PARTITIONING
// ============


void TestPartitioning::setUp() {}
void TestPartitioning::tearDown() {}
void TestPartitioning::test_partition_karger() {}


CppUnit::Test* TestPartitioning::suite() {

    CppUnit::TestSuite *test_suite =
        new CppUnit::TestSuite("TestPartitioning");
    test_suite->addTest(
        new CppUnit::TestCaller<TestPartitioning>(
            "test_partition_karger",
            &TestPartitioning::test_partition_karger
        )
    );
    return test_suite;
}

// SHORTEST PATH
// =============


void TestShortestPath::setUp() {}
void TestShortestPath::tearDown() {}
void TestShortestPath::test_get_shortest_path_dijkstra() {}


CppUnit::Test* TestShortestPath::suite() {

    CppUnit::TestSuite *test_suite =
        new CppUnit::TestSuite("TestShortestPath");
    test_suite->addTest(
        new CppUnit::TestCaller<TestShortestPath>(
            "test_get_shortest_path_dijkstra",
            &TestShortestPath::test_get_shortest_path_dijkstra
        )
    );
    return test_suite;
}