#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class Stopwatch {
public:
    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    Stopwatch() { start(); }
    void start() { start_ = now(); }

    size_t elapsedNanoseconds() const {
        time_point curr = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<
            std::chrono::nanoseconds
            >(curr - start_).count();
    }

private:
    static time_point now() { return std::chrono::high_resolution_clock::now(); }
    time_point start_;
};


#endif //STOPWATCH_H
