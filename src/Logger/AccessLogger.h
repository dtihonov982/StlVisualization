#ifndef ACCESSLOGGER_H
#define ACCESSLOGGER_H

#include <vector>
#include <iostream>
#include <chrono>

#include "Logger/Event.h"
#include "Common/Exception.h"
#include "Common/Script.h"

//AccessLogger logs changes in Container by using information aboud access to it.
//After each access AccessLogger compares it's own version of Container and
//original version of Container, that may have been changed. If there is difference
//then was changes.
template<typename Container>
class AccessLogger: public IEventHandler {
public:

    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    AccessLogger(const Container& original, std::ostream& os, const time_point& startPoint)
    : log_(os)
    , copy_(original)
    , original_(original)
    , startPoint_(startPoint)
    {
        if (getTime() < 0) 
            throw Exception("Start point must be in the past.");
    }

    AccessLogger(const Container& original, std::ostream& os)
    : AccessLogger(original, os, std::chrono::high_resolution_clock::now()) 
    {}

    void handle(Event& event) override {
        if (!isActive) 
            return;

        if (event.getType() != Event::Access)
            return;

        checkWriting();

        Access& accEvent = static_cast<Access&>(event);
        Action action{getTime(), Action::ACCESS, accEvent.getPos(), 0};
        script_.push_back(action);
        //log_ << getTime() << ",access," << accEvent.getPos() << '\n';
    }

    long long getTime() {
        auto currPoint_ = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(currPoint_ - startPoint_).count();
    }


    void checkWriting() {
        for (size_t i = 0; i < copy_.size(); ++i) {
            if (copy_[i] != original_[i]) {
                Action action{getTime(), Action::WRITE, i, original_[i]};
                //log_ << getTime() << ",write," << i << "," << original_[i] << '\n';
                copy_[i] = original_[i];
            }
        }
    }

    const Script& getScript() const {
        return script_;
    }

    void finalize() {
        checkWriting();
        isActive = false;

        for (auto& action: script_) {
            log_ << action.toString() << "\n";
        }
    }

private:
    bool isActive = true;
    std::ostream& log_;
    Script script_;

    Container copy_;
    const Container& original_;
    const time_point startPoint_;
};

#endif //ACCESSLOGGER_H

