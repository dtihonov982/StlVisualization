#ifndef EVENTINTERPRETER_H
#define EVENTINTERPRETER_H

#include <vector>
#include <iostream>
#include <chrono>

#include "Event.h"
#include "Exception.h"
#include "Script.h"

//1. Get event
//2. Register time of event
//3. Interpreter events to actions
template<typename Container>
class EventInterpreter: public IEventHandler {
public:

    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    EventInterpreter(const Container& original, const time_point& startPoint)
    : copy_(original)
    , original_(original) {
        if (getTime() < 0) 
            throw Exception("Start point must be in the past.");
    }

    //TODO: default value
    EventInterpreter(const Container& original)
    : EventInterpreter(original, std::chrono::high_resolution_clock::now()) 
    {}

    void handle(Event& event) override {
        if (event.getType() != Event::Access)
            return;

        checkWriting();

        Access& accEvent = static_cast<Access&>(event);
        Action action{getTime(), Action::ACCESS, accEvent.getPos(), 0};
        script_.push_back(action);
    }

    const Script& getScript() {
        checkWriting();
        return script_;
    }

private:
    long long getTime() {
        auto currPoint_ = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(currPoint_ - startPoint_).count();
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
    //Stopwatch stopwatch_;
};

#endif //EVENTINTERPRETER_H
