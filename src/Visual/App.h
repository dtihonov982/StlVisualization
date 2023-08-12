#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include "Chart.h"
#include "Journal.h"
#include <stack>

class App {
public:
    enum Status { Play, Pause, Done };
    //TODO: Config file
    App(std::string_view logfile);
    ~App();
    void update();
    void render();
    void handleEvents();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::string title_;
    std::vector<int> data_;
    std::stack<int> markedPos_;
    Journal journal_;
    typename Journal::iterator currentEntry_;
    Chart chart_;
    bool isRunning_ = false;
    //Runs paused. Starts with Space.
    Status status_ = Pause;
};
#endif // APP_H
