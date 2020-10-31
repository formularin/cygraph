/*
Implementation of algorithm test fixtures.
*/

#include "../include/test_algorithms.hpp"
#include "../include/test_graph.hpp"


void TestArticulationPoints::setUp() {}

void TestArticulationPoints::tearDown() {}

void TestArticulationPoints::test_get_articulation_points() {
    CPPUNIT_ASSERT( 1 == 1 );
}

CppUnit::Test* TestArticulationPoints::suite() {
    /*
    Creates a suite of tests containing all of the test methods in this
    class.
    */

    CppUnit::TestSuite *test_suite =
        new CppUnit::TestSuite("TestArticulationPoints");
    test_suite->addTest(
            new CppUnit::TestCaller<TestArticulationPoints>(
                "test_get_articulation_points",
                &TestArticulationPoints::test_get_articulation_points));
    return test_suite;
}