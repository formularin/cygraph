/*
Common functions and classes for unit tests.

TODO: Move UserDefinedObject implementations to implementation file.
*/

#ifndef CYGRAPH_TEST_UTILS_HPP
#define CYGRAPH_TEST_UTILS_HPP

#include <string>
#include <unordered_map>


namespace cygraph_utils {

    class UserDefinedObject {
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

}

// Override std::hash for UserDefinedObject
namespace std {
    template <> struct hash<cygraph_utils::UserDefinedObject> {
        size_t operator()(const cygraph_utils::UserDefinedObject& object) const;
    };
}

#endif