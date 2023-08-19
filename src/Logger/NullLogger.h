#ifndef NULLLOGGER_H
#define NULLLOGGER_H

#include <Logger/Event.h>

class NullLogger: public IEventHandler {
public:
    void handle(Event& e) override {}
};

#endif //NULLLOGGER_H
