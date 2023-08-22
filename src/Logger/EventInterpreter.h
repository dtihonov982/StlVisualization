#ifndef EVENTINTERPRETER_H
#define EVENTINTERPRETER_H

#include <vector>
#include <iostream>
#include <chrono>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <mutex>

#include "Logger/Event.h"
#include "Common/Exception.h"
#include "Common/Script.h"
#include "Logger/Stopwatch.h"

// EventInterpreter gets from NotifyingIterator an events and interpreters those events as an access and a changes of data.
template<typename Container>
class EventInterpreter: public IEventHandler {
public:

    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    // Class uses shared_ptr to stopwatch, because for one algorithm may be many EventInterpreter for each data
    // As example, the algorithm std::copy(f1, l1, f2) gets two diapasons: [f1, l1) and [f2, l2).
    // For each of this diapasons may be it's own EventIterpreter. But their stopwatches will be synchronized.
    EventInterpreter(const std::shared_ptr<Container>& original, const std::shared_ptr<Stopwatch>& stopwatch)
    : original_(original) 
    , stopwatch_(stopwatch) {
        if (!original)
            throw Exception("Pointer to original must be initialized.");
        copy_ = *original;
    }

    // TODO: default argument
    // For single diapason algorithm (std::sort) there is no need to synchronize stopwatch
    EventInterpreter(const std::shared_ptr<Container>& original)
    : EventInterpreter(original, std::make_shared<Stopwatch>()) 
    {}

    // A NotifyingIterator can notify only about access. The goal is to get information about writing.
    // Also it is important to get information about when was writing. The main idea behind EventInterpreter is that
    // writing can be only between two access from NotifyingIterator to the data. So if we will check if the data was
    // changed between two access from NotifyingIterator we find an approximately moment of writing.
    void handle(Event& event) override {
        if (event.getType() != Event::Access)
            return;

        // The time of interpretation of events must not interact to information about writing and access time.
        pause_guard pause(*stopwatch_);

        checkWriting();

        Access& accEvent = static_cast<Access&>(event);
        Action action{stopwatch_->elapsedNanoseconds(), Action::ACCESS, accEvent.getPos(), 0};
        script_.push_back(action);

    }

    const Script& getScript() {
        pause_guard pause(*stopwatch_);
        checkWriting();
        return script_;
    }

private:
    // TODO: save in class member
    bool getLastAccess(Action& dst) const {
        auto it = std::find_if(script_.crbegin(), script_.crend(),
            [] (const Action& action) {
                return action.type == Action::ACCESS;
            });
        if (it != script_.crend()) {
            dst = *it;
            return true;
        }
        else {
            return false;
        }
    }

    void checkWriting() {
        Action lastAccess;
        uint64_t writingPoint;
        if (getLastAccess(lastAccess)) {
            writingPoint = lastAccess.timePoint;
        }
        else {
            writingPoint = stopwatch_->elapsedNanoseconds();
        }
        //TODO:delete last access if positions of writing and access are the same.
        for (size_t i = 0; i < copy_.size(); ++i) {
            auto mutableContainerValue = (*original_)[i];
            if (copy_[i] != mutableContainerValue) {
                Action action{writingPoint, Action::WRITE, i, mutableContainerValue};
                script_.push_back(action);
                copy_[i] = mutableContainerValue;
            }
        }
    }

    Script script_;
    const time_point startPoint_;

    Container copy_;
    std::shared_ptr<Container> original_;
    std::shared_ptr<Stopwatch> stopwatch_;
};

#endif //EVENTINTERPRETER_H
