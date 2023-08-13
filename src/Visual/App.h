#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include "Chart.h"
#include "Script.h"
#include <stack>
#include "Player.h"
#include "Exception.h"

class App {
public:
    App(const std::vector<std::string_view>& files);
    ~App();
    void update();
    void render();
    void handleEvents();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
    static std::vector<SDL_Rect>
    emplaceBlocks(int count, 
                  int width, 
                  int height, 
                  float gapRatio = 0.90f);
private:
    std::vector<Player> players_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    bool isRunning_ = false;
};
#endif // APP_H
