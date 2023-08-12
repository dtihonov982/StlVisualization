#include <iostream>

#include "DebugLogger.h"

void DebugLogger::handle(Event& event) {
    switch (event.getType()) {
        case Event::Message: 
            Message& msg = static_cast<Message&>(event);
            auto text = msg.getText();
#if 0
            std::cout << text << '\n';
#else
            auto it = unique_.find(text);
            if (it == unique_.end()) {
                unique_.insert(text);
                std::cout << text << '\n';
            }
#endif
            break;
    }
}


