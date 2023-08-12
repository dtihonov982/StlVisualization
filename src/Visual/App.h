#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include "Chart.h"
#include "Script.h"
#include <stack>
#include "Player.h"

class App {
public:
    App(std::string_view logfile);
    ~App();
    void update();
    void render();
    void handleEvents();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
private:
    std::vector<Player> players_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    bool isRunning_ = false;
};
#endif // APP_H
