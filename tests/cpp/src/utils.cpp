/*
Common functions and classes for unit tests.
*/

#include <string>
#include <unordered_map>

#include "../include/utils.hpp"

using namespace cygraph_utils;

const std::string UserDefinedObject::type_name = "UserDefinedObject";
const std::string Integer::type_name = "Integer";
const std::string Long::type_name = "Long";
const std::string Float::type_name = "Float";
const std::string Double::type_name = "Double";
const std::string Bool::type_name = "Bool";
const std::string Char::type_name = "Char";
const std::string String::type_name = "String";

// Overriding std::hash for UserDefinedObject.
std::size_t std::hash<UserDefinedObject>::operator()(const UserDefinedObject& object) const {
    std::size_t a_hash = std::hash<int>()(object.get_a());
    std::size_t b_hash = std::hash<int>()(object.get_b()) << 1;
    return a_hash ^ b_hash;
}