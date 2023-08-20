#ifndef SCHEDULER_H
#define SCHEDULER_H

class Executor;
class Scheduler;

struct Task {
    std::shared_ptr<Executor> executor;
    std::string data;
    size_t timeUntil;
};

bool operator<(const Task& lhs, const Task& rhs) {
    return lhs.timeUntil < rhs.timeUntil;
}

class Executor {
public:
    void execute(Task& task) {
        std::cout << "Execution of: " << task.data << "\n";
    }
    void plan() {
        std::string data{"New task."};
        //how store to ptr on Executor?
        Task task{this, data, 1000};
        scheduler_->add(task);
    }
private:
    Scheduler* scheduler_;
};

class Scheduler {
public:
    void add(Task& task) {
        tasks_.insert(task);
    }
    void run() {
        while (!tasks_.empty()) {
            Task& curr = *tasks_.begin();
            curr->executor->execute(curr);

        }
    }
private:
    std::set<Task> tasks_;
};


#endif //SCHEDULER_H
