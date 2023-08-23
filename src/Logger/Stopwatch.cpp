#include "Logger/Stopwatch.h"

using namespace std::chrono;

void Stopwatch::start() { 
    start_ = clock::now(); 
    pausePoint_ = start_;
    idle_ = nanoseconds::zero();
    isRunning_ = true;
}

void Stopwatch::pause() {
    if (isRunning_) {
        pausePoint_ = clock::now();
        isRunning_ = false;
    }
}

void Stopwatch::resume() {
    if (!isRunning_) {
        idle_ += clock::now() - pausePoint_;
        isRunning_ = true;
    }
}

uint64_t Stopwatch::elapsedNanoseconds() const {
    time_point currPoint;
    if (isRunning_) {
        currPoint = clock::now();
    }
    else {
        currPoint = pausePoint_;
    }
    nanoseconds duration = (currPoint - start_) - idle_;
    assert(duration >= nanoseconds::zero());
    return static_cast<uint64_t>(duration.count());
}

