#include <chrono>
#include <set>
#include <thread>

#include "Exception.h"


template <typename Task>
struct earlier {
    bool operator()(const Task& lhs, const Task& rhs) const {
        return lhs.point < rhs.point;
    }
};

template <typename Job>
class Scheduler {
public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = std::chrono::time_point<clock>;

    struct Task {
        Task(time_point p, const Job& j)
        : point(p), job(j) 
        {}
        time_point point;
        Job job;
    };

    void add(std::chrono::milliseconds timeUntil, const Job& job) {
        units_.emplace(clock::now() + timeUntil, job);
    }
    bool empty() {
        return units_.empty();
    }
    Job wait() {
        if (!empty()) {
            Task curr = *units_.begin();
            units_.erase(units_.begin());
            auto delay = curr.point - clock::now();
            if (delay < std::chrono::milliseconds::zero())
                delay = std::chrono::milliseconds::zero();
            std::this_thread::sleep_for(delay);
            return curr.job;
        }
        else {
            throw Exception("Scheduler queue is empty.");
        }
    }
private:
    std::set<Task, earlier<Task>> units_;
};
