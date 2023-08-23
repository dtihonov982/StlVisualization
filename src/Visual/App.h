#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <string_view>
#include <string>
#include <stack>
#include <cstdint>

#include "Visual/Player.h"
#include "Common/Exception.h"
#include "Visual/Chart.h"
#include "Visual/Scheduler.h"
#include "Common/Script.h"
#include "Logger/Event.h"

using PlayerScheduler = Scheduler<size_t>;

class App: public IEventHandler {
public:
    App(uint64_t delayRatio, const std::vector<std::string_view>& files);
    ~App();
    void run();
    void handleEvents();
    void update();
    void render();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
    static std::vector<SDL_Rect>
    emplaceBlocks(int count, 
                  int width, 
                  int height, 
                  float gapRatio = 0.90f);
    void initScheduler();
    void handle(Event& event) override;
private:
    Scheduler<IEventHandlerPtr> sched_;
    std::vector<Player> players_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    bool isRunning_ = false;
};
#endif // APP_H
