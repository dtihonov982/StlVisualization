#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    enum Type { Message };  
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

class Message: public Event {
public:
    Message(std::string text)
    : Event(Event::Message)
    , text_(text) {}
    std::string getText() { return text_; }
private:
    std::string text_;
};

#endif //EVENT_H
