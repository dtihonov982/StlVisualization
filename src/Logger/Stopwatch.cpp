#include "Stopwatch.h"

using namespace std::chrono;

void Stopwatch::start() { 
    start_ = high_resolution_clock::now(); 
    idle_ = 0;
    isRunning_ = true;
}

void Stopwatch::pause() {
    if (isRunning_) {
        pausePoint_ = high_resolution_clock::now();
        isRunning_ = false;
    }
}

void Stopwatch::resume() {
    if (!isRunning_) {
        idle_ += getNanosecondsSince(pausePoint_);
        isRunning_ = true;
    }
}

size_t Stopwatch::getNanosecondsSince(const time_point& point) {
    auto count = duration_cast<nanoseconds>(high_resolution_clock::now() - point).count();
    assert(count >= 0);
    return static_cast<size_t>(count);
}

size_t Stopwatch::elapsedNanoseconds() const {
    if (isRunning_) {
        return getNanosecondsSince(start_) - idle_;
    }
    else {
        auto count = duration_cast<nanoseconds>(pausePoint_ - start_).count();
        assert(count >= 0);
        return static_cast<size_t>(count);
    }
}

