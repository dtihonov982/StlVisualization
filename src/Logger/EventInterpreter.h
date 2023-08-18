#ifndef EVENTINTERPRETER_H
#define EVENTINTERPRETER_H

#include <vector>
#include <iostream>
#include <chrono>
#include <memory>

#include "Event.h"
#include "Exception.h"
#include "Script.h"
#include "Stopwatch.h"

//1. Get event
//2. Register time of event
//3. Interpreter events to actions
template<typename Container>
class EventInterpreter: public IEventHandler {
public:

    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    EventInterpreter(const Container& original, const std::shared_ptr<Stopwatch>& stopwatch)
    : copy_(original)
    , original_(original) 
    , stopwatch_(stopwatch) {
        if (getTime() < 0) 
            throw Exception("Start point must be in the past.");
    }

    //TODO: default value
    EventInterpreter(const Container& original)
    : EventInterpreter(original, std::make_shared<Stopwatch>()) 
    {}

    void handle(Event& event) override {
        if (event.getType() != Event::Access)
            return;

        stopwatch_->pause();

        checkWriting();

        Access& accEvent = static_cast<Access&>(event);
        Action action{getTime(), Action::ACCESS, accEvent.getPos(), 0};
        script_.push_back(action);

        stopwatch_->resume();
    }

    const Script& getScript() {
        stopwatch_->pause();
        checkWriting();
        stopwatch_->resume();
        return script_;
    }

private:
    size_t getTime() {
        return stopwatch_->elapsedNanoseconds();
    }


    void checkWriting() {
        for (size_t i = 0; i < copy_.size(); ++i) {
            if (copy_[i] != original_[i]) {
                Action action{getTime(), Action::WRITE, i, original_[i]};
                script_.push_back(action);
                copy_[i] = original_[i];
            }
        }
    }

    Script script_;
    const time_point startPoint_;

    Container copy_;
    const Container& original_;
    std::shared_ptr<Stopwatch> stopwatch_;
};

#endif //EVENTINTERPRETER_H
