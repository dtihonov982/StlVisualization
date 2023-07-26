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
    //TODO: Config file
    App(std::string_view logfile);
    ~App();
    void update();
    void render();
    void handleEvents();
    bool isRunning() { return isRunning_; }
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::string title_;
    std::vector<int> data_;
    std::stack<int> markedPos_;
    Journal journal_;
    typename Journal::iterator currentEntry_;
    Chart chart_;
    //TODO: more states?
    bool isRunning_ = false;
};
#endif // APP_H
