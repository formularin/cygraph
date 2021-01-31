/* ============================================================================================== *
 * utils.cpp                                                                                      *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * Implementations of common functions and classes necessary for CyGraph unit testing.            *
 * ============================================================================================== */


#include <iostream>
#include <string>
#include <unordered_map>

#include "../include/utils.hpp"


// Overriding std::hash for UserDefinedObject.
std::size_t std::hash<UserDefinedObject>::operator()(const UserDefinedObject& object) const {
    std::size_t a_hash = std::hash<int>()(object.get_a());
    std::size_t b_hash = std::hash<int>()(object.get_b()) << 1;
    return a_hash ^ b_hash;
}


// Overriding << for std::ostream and UserDefinedObject
std::ostream& operator<<(std::ostream& target, const UserDefinedObject& source) {
    target << source.a << " " << source.b;
    return target;
}