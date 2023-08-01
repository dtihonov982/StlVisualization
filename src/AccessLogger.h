#ifndef ACCESSLOGGER_H 
#define ACCESSLOGGER_H

#include <vector>
#include <iostream>

#include "Event.h"

template<typename Container>
class AccessLogger: public IEventHandler {
public:

    AccessLogger(const Container& original, std::ostream& os)
    : log_(os)
    , copy_(original)
    , original_(original)
    {}

    void handle(Event& event) override {
        if (event.getType() != Event::Access)
            return;

        checkWriting();

        Access& accEvent = static_cast<Access&>(event);
        log_ << "access," << accEvent.getPos() << '\n';
    }


    void checkWriting() {
        for (size_t i = 0; i < copy_.size(); ++i) {
            if (copy_[i] != original_[i]) {
                log_ << "change," << i << "," << original_[i] << '\n';
                copy_[i] = original_[i];
            }
        }
    }

    void finalize() {
        checkWriting();
    }

private:
    std::ostream& log_;

    Container copy_;
    const Container& original_;
};

#endif //ACCESSLOGGER_H

