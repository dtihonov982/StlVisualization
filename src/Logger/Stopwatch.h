#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <cassert>
#include <cstdint>

class Stopwatch {
public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = std::chrono::time_point<clock>;

    Stopwatch() { start(); }
    void start();
    void pause();
    void resume();
    uint64_t elapsedNanoseconds() const;
private:
    std::chrono::nanoseconds idle_;
    time_point start_;
    time_point pausePoint_;
    bool isRunning_;
};


#endif //STOPWATCH_H
