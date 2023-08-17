#include <iostream>

#include "Logger/Stopwatch.h"

int main() {
    Stopwatch sw;
    std::cout << sw.elapsedNanoseconds() << "\n";
    sw.start();
    std::cout << sw.elapsedNanoseconds() << "\n";
    return 0;
}

