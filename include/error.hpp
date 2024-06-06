#include <iostream>
#include <string>

namespace Error {
    inline void report(int line, std::string message) {
        std::cerr << "[line: " << line << "] Error: " + message;
    }
}