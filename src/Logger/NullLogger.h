#ifndef NULLLOGGER_H
#define NULLLOGGER_H

#include <Logger/Event.h>

// If for some data in algorithm input is no need to make a recod, then use NullLogger.
class NullLogger: public IEventHandler {
public:
    void handle(Event& e) override {}
};

#endif //NULLLOGGER_H
