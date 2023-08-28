#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <cassert>
#include <cstdint>
#include <mutex>

class Stopwatch {
public:
    using clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<clock>;

    Stopwatch() { start(); }
    void start();
    void pause();
    // Synonym for lock_guard
    void lock() { pause(); }
    void resume();
    // Synonym for lock_guard
    void unlock() { resume(); }
    uint64_t elapsedNanoseconds() const;
private:
    // Then stopwatch is idle this value grows.
    // Idle doesn't affect on duration.
    std::chrono::nanoseconds idle_;
    time_point start_;
    // A moment when was last pause. It is valid only in pause state.
    time_point pausePoint_;
    // The Stopwatch can be only running or paused.
    bool isRunning_;
};

// Using a lock_guard to automatically resume a stopwatch after scope exit
using pause_guard = std::lock_guard<Stopwatch>;


#endif //STOPWATCH_H
