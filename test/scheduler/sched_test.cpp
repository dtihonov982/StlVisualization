#include "Scheduler.h"
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono_literals;
using namespace std::chrono;

struct Worker {
    milliseconds period;
    int id;
    void plan(Scheduler<int>& S) {
        S.add(period, id);
    }
};

int getNext(Scheduler<int>& S) {
    auto start = std::chrono::high_resolution_clock::now();
    auto job = S.wait();
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    auto count = duration.count();
    std::cout << count << "\t\t" << job << "\n";
    return job;
}

int main() {
    std::cout << "Waiting, ms:\tresult:\n";
    Scheduler<int> S;
    S.add(-5000ms, 1000);
    getNext(S);
    std::vector<Worker> pool {Worker {300ms, 0}, Worker {250ms, 1}, Worker {500ms, 2}};
    for (auto& w: pool)
        w.plan(S);
    while (!S.empty()) {
        int job = getNext(S);
        pool[job].plan(S);
    }
    return 0;
}
