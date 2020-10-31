/*
Common functions and classes for unit tests.
*/

#include <string>
#include <unordered_map>

#include "../include/utils.hpp"


using namespace cygraph_utils;


// Overriding std::hash for UserDefinedObject.
std::size_t std::hash<UserDefinedObject>::operator()(const UserDefinedObject& object) const {
    std::size_t a_hash = std::hash<int>()(object.get_a());
    std::size_t b_hash = std::hash<int>()(object.get_b()) << 1;
    return a_hash ^ b_hash;
}