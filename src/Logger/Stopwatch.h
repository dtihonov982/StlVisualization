#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <cassert>

class Stopwatch {
public:
    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    Stopwatch() { start(); }
    void start();
    void pause();
    void resume();
    static size_t getNanosecondsSince(const time_point& point);
    size_t elapsedNanoseconds() const;
private:
    size_t idle_;
    time_point start_;
    time_point pausePoint_;
    bool isRunning_;
};


#endif //STOPWATCH_H
