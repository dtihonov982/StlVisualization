#include "Common/Script.h"
#include <iostream>

int main(int argc, char** argv) 
try {
    if (argc < 2)
        return 0;

    Action a1 = Action::loadFromString(argv[1]);
    std::cout << a1.toString();
    return 0;
}
catch (const std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
}
