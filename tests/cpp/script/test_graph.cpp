/*
Tests the Graph classes.
*/

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "../lib/catch.hpp"
#include "../../../include/cygraph.hpp"
#include "../include/utils.hpp"


using namespace cygraph;
using cygraph_utils::UserDefinedObject;


using TemplateTypes = std::tuple<std::tuple<cygraph_utils::Integer, cygraph_utils::Integer>, std::tuple<cygraph_utils::Integer, cygraph_utils::Long>, std::tuple<cygraph_utils::Integer, cygraph_utils::Float>, std::tuple<cygraph_utils::Integer, cygraph_utils::Double>, std::tuple<cygraph_utils::Integer, cygraph_utils::Bool>, std::tuple<cygraph_utils::Char, cygraph_utils::Integer>, std::tuple<cygraph_utils::Char, cygraph_utils::Long>, std::tuple<cygraph_utils::Char, cygraph_utils::Float>, std::tuple<cygraph_utils::Char, cygraph_utils::Double>, std::tuple<cygraph_utils::Char, cygraph_utils::Bool>, std::tuple<cygraph_utils::String, cygraph_utils::Integer>, std::tuple<cygraph_utils::String, cygraph_utils::Long>, std::tuple<cygraph_utils::String, cygraph_utils::Float>, std::tuple<cygraph_utils::String, cygraph_utils::Double>, std::tuple<cygraph_utils::String, cygraph_utils::Bool>, std::tuple<UserDefinedObject, cygraph_utils::Integer>, std::tuple<UserDefinedObject, cygraph_utils::Long>, std::tuple<UserDefinedObject, cygraph_utils::Float>, std::tuple<UserDefinedObject, cygraph_utils::Double>, std::tuple<UserDefinedObject, cygraph_utils::Bool>>;

std::vector<cygraph_utils::Integer> int_values = {0, 1, 2, 3, 4};
std::vector<cygraph_utils::Long> long_values = {0, 1, 2, 3, 4};
std::vector<cygraph_utils::Float> float_values = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
std::vector<cygraph_utils::Double> double_values = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
std::vector<cygraph_utils::Bool> bool_values = {false, true, false, true, false};
std::vector<cygraph_utils::Char> char_values = {'a', 'b', 'c', 'd', 'e'};
std::vector<cygraph_utils::String> string_values = {(std::string) "", (std::string) "a", (std::string) "ab", (std::string) "abc", (std::string) "abcd"};
std::vector<cygraph_utils::UserDefinedObject> object_values = {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}};


TEMPLATE_LIST_TEST_CASE("Adding and removing edges to adjacency matrix graphs", "[edge]", TemplateTypes ) {}
