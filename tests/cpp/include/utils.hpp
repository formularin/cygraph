/* ============================================================================================== *
 * utils.hpp                                                                                      *
 * authors: lol-cubes                                                                             *
 *                                                                                                *
 * Common functions and classes for unit test implementations.                                    *
 * ============================================================================================== */


#ifndef CYGRAPH_TEST_UTILS_HPP
#define CYGRAPH_TEST_UTILS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


using std::vector;


class UserDefinedObject {
    friend std::ostream& operator<<(std::ostream& target, const UserDefinedObject& source);
    private:
        int a, b;
    public:
        const static std::string type_name;
        UserDefinedObject(): a(0), b(0) {};
        UserDefinedObject(int a, int b): a(a), b(b) {};
        int get_a() const {return a;}
        int get_b() const {return b;}
        bool operator==(const UserDefinedObject& other) const {
            return ( a == other.get_a() && b == other.get_b() );
        }
};


// Sample values.
const vector<int> int_vals = {-1, 0, 1, 7};
const vector<float> float_vals = {-1.0f, -0.5f, 0.0f, 0.5f};
const vector<std::string> string_vals = {"", "Mumbai", "New York", "Tokyo"};
const vector<bool> bool_vals = {true, false, true, false};
const vector<UserDefinedObject> object_vals = {
    UserDefinedObject(), UserDefinedObject(-2, 0),
    UserDefinedObject(1, 1), UserDefinedObject(1, 2)
};


// Override std::hash for UserDefinedObject
namespace std {
    template <> struct hash<UserDefinedObject> {
        size_t operator()(const UserDefinedObject& object) const;
    };
}
// Override << for UserDefinedObject and std::ostream
std::ostream& operator<<(std::ostream& target, const UserDefinedObject& source);

#endif