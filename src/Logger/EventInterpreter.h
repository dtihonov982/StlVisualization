#ifndef EVENTINTERPRETER_H
#define EVENTINTERPRETER_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <mutex>

#include "Common/Exception.h"
#include "Common/Record.h"
#include "Logger/Stopwatch.h"
#include "Logger/Event.h"

// EventInterpreter gets from NotifyingIterator an events and interpreters those events as an access and a changes of data.
template <typename Container>
class EventInterpreter: public IEventHandler {
public:

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
        if (recordingIsPaused_ )
            return;
        // The time of interpretation of events must not interact to information about writing and access time.
        pause_guard pause(*stopwatch_);
        // Before check writing script is empty or last elements of it is Access.
        checkWriting();
        addAccess(static_cast<Access&>(event));
    }

    void addAccess(Access& access) {
        Action action{stopwatch_->elapsedNanoseconds(), Action::ACCESS, access.getPos(), 0};
        script_.push_back(action);
    }

    const Script& getScript() {
        pause_guard pause(*stopwatch_);
        checkWriting();
        return script_;
    }

    void pauseRecording() { recordingIsPaused_ = true; }
    void resumeRecording() { recordingIsPaused_ = false; }

private:
    using value_type = typename Container::value_type;

    void checkWriting() {
        // Time point for writing - time of last access
        uint64_t writingPoint;
        if (!script_.empty()) {
            auto lastAccess = script_.end() - 1;
            writingPoint = lastAccess->timePoint;
            auto lastAccessPos = lastAccess->pos;
            // Delete last access if after it was writing.
            if ( (*original_)[lastAccessPos] != copy_[lastAccessPos] )
                script_.pop_back();
        }
        else {
            writingPoint = stopwatch_->elapsedNanoseconds();
        }
        // Scan container if was changes and where it was.
        for (size_t i = 0; i < copy_.size(); ++i) {
            auto mutableContainerValue = (*original_)[i];
            if (copy_[i] != mutableContainerValue) {
                // If was changes then add Write action in script
                Action action{writingPoint, Action::WRITE, i, mutableContainerValue};
                script_.push_back(action);
                // update copy to container for correct next write checking.
                copy_[i] = mutableContainerValue;
                // if writing pos and last access pos is the same then delete access action
            }
        }
    }

    bool recordingIsPaused_ = false;
    Script script_;
    Container copy_;
    std::shared_ptr<Container> original_;
    std::shared_ptr<Stopwatch> stopwatch_;
};

#endif //EVENTINTERPRETER_H
