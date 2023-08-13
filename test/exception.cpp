#include "Exception.h"
#include <iostream>
#include <sstream>

template<typename... Args>
std::string concate(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

int main() {
    try {
        throw Exception("abc", "def", 123);
    }
    catch (const Exception& ex) {
        std::cout << ex.what() << "\n";
    }
    return 0;
}
