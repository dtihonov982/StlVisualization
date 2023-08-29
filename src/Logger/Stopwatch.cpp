#include <time.h>
#include <cassert>
#include "Logger/Stopwatch.h"

long now() {
    static timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    return ts.tv_nsec;
}

void Stopwatch::start() { 
    start_ = now(); 
    pausePoint_ = start_;
    idle_ = 0L;
    isRunning_ = true;
}

void Stopwatch::pause() {
    if (isRunning_) {
        pausePoint_ = now();
        isRunning_ = false;
    }
}

void Stopwatch::resume() {
    if (!isRunning_) {
        idle_ += now() - pausePoint_;
        isRunning_ = true;
    }
}

uint64_t Stopwatch::elapsedNanoseconds() const {
    time_point currPoint;
    if (isRunning_) {
        currPoint = now();
    }
    else {
        currPoint = pausePoint_;
    }
    duration dur = (currPoint - start_) - idle_;
    assert(dur >= 0L);
    return static_cast<uint64_t>(dur);
}

