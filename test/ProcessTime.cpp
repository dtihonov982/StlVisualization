#include <time.h>
#include <chrono>
#include <thread>
#include <iostream>

using chronoClock = std::chrono::steady_clock;
auto base = chronoClock::now();

long now_Process() {
    static timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    return ts.tv_nsec;
}

long now_Thread() {
    static timespec ts;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    return ts.tv_nsec;
}

long now_Monotonic() {
    static timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_nsec;
}

long now_Chrono() {
    auto point = chronoClock::now();
    return (point - base).count();
}

using TimeFunc = long (*) ();

void measure(TimeFunc func) {
    auto start = func();
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    auto finish = func();
    std::cout << finish - start << std::endl;
}

int main() {
    measure(now_Process);
    measure(now_Thread);
    measure(now_Monotonic);
    measure(now_Chrono);
    return 0;
}
