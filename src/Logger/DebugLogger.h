#ifndef DEBUG_LOGGER
#define DEBUG_LOGGER

#include <unordered_set>
#include <string>

#include "Event.h"

//DebugLogger collects information about which method of 
//iterator was called.
class DebugLogger: public IEventHandler {
public:
    void handle(Event& event) override;
private:
    std::unordered_set<std::string> unique_;
};

#endif
