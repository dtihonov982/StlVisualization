#ifndef DEBUG_LOGGER
#define DEBUG_LOGGER

#include <unordered_set>
#include <string>

#include "Event.h"

class DebugLogger: public IEventHandler {
public:
    void handle(Event& event) override;
private:
    std::unordered_set<std::string> unique_;
};

#endif
