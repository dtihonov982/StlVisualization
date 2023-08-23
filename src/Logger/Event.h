#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <memory>

#include "Visual/Scheduler.h"

class Event {
public:
    enum Type { Message, Access, WakeUp};  
    Type getType() { return type_; }  
    virtual ~Event() {}
protected:
    Event(Type type): type_(type) {}
private:
    Type type_;
};

class IEventHandler {
public:
    virtual void handle(Event& event) = 0;
    virtual ~IEventHandler() {}
};

using IEventHandlerPtr = IEventHandler*;

class Message: public Event {
public:
    Message(std::string text)
    : Event(Event::Message)
    , text_(text) {}
    std::string getText() { return text_; }
private:
    std::string text_;
};

class Access: public Event {
public:
    Access(size_t pos)
    : Event(Event::Access)
    , pos_(pos) {}
    size_t getPos() const { return pos_; }
private:
    size_t pos_;
};

struct WakeUp: public Event {
    WakeUp(const SchedulerPtr<IEventHandlerPtr>& scheduler)
    : Event(Event::WakeUp) 
    , sched(scheduler) 
    {}
    SchedulerPtr<IEventHandlerPtr> sched;
};

#endif //EVENT_H
