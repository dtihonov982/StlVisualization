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
    void render();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
    void initScheduler();
    void initGraphics();
    void createPlayers(uint64_t delayRatio, const std::vector<std::string_view>& files);
    void handle(Event& event) override;
private:
    Scheduler<IEventHandlerPtr> sched_;
    std::vector<Player> players_;
    SDL_Window* window_;
    int windowWidth_;
    int windowHeight_;
    SDL_Renderer* renderer_;
    bool isRunning_ = false;
};

static std::vector<SDL_Rect>
getGrid(int count, int width, int height, float gapRatio = 0.90f);

#endif // APP_H
