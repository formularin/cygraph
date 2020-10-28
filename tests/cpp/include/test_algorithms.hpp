/*
Test fixtures required for testing algorithm functions.
*/

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

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
