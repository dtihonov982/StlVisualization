#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <cstdint>
#include <mutex>

class Stopwatch {
public:
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
    using time_point = long;
    using duration = long;
    // Then stopwatch is idle this value grows.
    // Idle doesn't affect on duration.
    duration idle_;
    time_point start_;
    // A moment when was last pause. It is valid only in pause state.
    time_point pausePoint_;
    // The Stopwatch can be only running or paused.
    bool isRunning_;
};

// Using a lock_guard to automatically resume a stopwatch after scope exit
using pause_guard = std::lock_guard<Stopwatch>;


#endif //STOPWATCH_H
