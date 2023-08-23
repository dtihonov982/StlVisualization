#include <iostream>

#include "Logger/Stopwatch.h"

int main() {
    Stopwatch sw;
    auto ctorDuration = sw.elapsedNanoseconds();
    //time from start in ctor
    std::cout << "Ctor: " << ctorDuration << "\n";

    sw.start();
    auto startDuration = sw.elapsedNanoseconds();
    //time from start
    std::cout << "Start: " << startDuration << "\n";

    //time of loop for further using
    sw.start();
    for (int i = 0; i < 1'000'000; ++i);
    auto loopDuration = sw.elapsedNanoseconds();
    std::cout << "Loop: " << loopDuration << "\n";

    //test pause resume
    sw.start();
    sw.pause();
    for (int i = 0; i < 1'000'000; ++i);
    sw.resume();
    auto loopPausedDuration = sw.elapsedNanoseconds();
    std::cout << "Pause in loop: " << loopPausedDuration << "\n";

    //pause resume cost
    sw.start();
    sw.pause();
    sw.resume();
    auto pauseCost = sw.elapsedNanoseconds();
    std::cout << "Pause resume cost: " << pauseCost << "\n";

    return 0;
}

