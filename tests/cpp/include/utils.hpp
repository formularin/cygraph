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

    // Type wrappers so that type name can be accessed for conditionals.

    class Integer {
        private:
            int value;
        public:
            const static std::string type_name;
            Integer(int value): value(value) {};
            Integer(): value(0) {};
            int get_value() {return value;}
    };

    class Long {
        private:
            long value;
        public:
            const static std::string type_name;
            Long(long value): value(value) {};
            Long(): value(0) {};
            long get_value() {return value;}
    };

    class Float {
        private:
            float value;
        public:
            const static std::string type_name;
            Float(float value): value(value) {};
            Float(): value(0.0f) {};
            float get_value() {return value;}
    };

    class Double {
        private:
            double value;
        public:
            const static std::string type_name;
            Double(double value): value(value) {};
            Double(): value(0.0) {};
            double get_value() {return value;}
    };

    class Bool {
        private:
            bool value;
        public:
            const static std::string type_name;
            Bool(bool value): value(value) {};
            Bool(): value(false) {};
            bool get_value() {return value;}
    };

    class Char {
        private:
            char value;
        public:
            const static std::string type_name;
            Char(char value): value(value) {};
            Char(): value(' ') {};
            bool get_value() {return value;}
    };

    class String {
        private:
            std::string value;
        public:
            const static std::string type_name;
            String(std::string value): value(value) {};
            String(): value((std::string) "") {};
            std::string get_value() {return value;}
    };
}

// Override std::hash for UserDefinedObject
namespace std {
    template <> struct hash<cygraph_utils::UserDefinedObject> {
        std::size_t operator()(const cygraph_utils::UserDefinedObject& object) const;
    };
}

#endif