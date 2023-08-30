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
#include "Common/Record.h"
#include "Logger/Event.h"
#include "Common/Config.h"

using PlayerScheduler = Scheduler<size_t>;

class App: public IEventHandler {
public:
    App(float delayRatio, const std::vector<std::string_view>& files, const Config& config);
    ~App();
    void run();
    void handleEvents();
    void render();
    void handleKeyDown(SDL_Event& event);
    bool isRunning() { return isRunning_; }
    void initScheduler();
    void initGraphics();
    void createPlayers(float delayRatio, const std::vector<std::string_view>& files);
    void handle(Event& event) override;
private:
    Scheduler<IEventHandlerPtr> sched_;
    std::vector<Player> players_;
    std::shared_ptr<Config> config_;
    SDL_Window* window_;
    SDL_Color backgroundColor_;
    int windowWidth_;
    int windowHeight_;
    SDL_Renderer* renderer_;
    bool isRunning_ = false;
};

static std::vector<SDL_Rect>
getGrid(int count, int width, int height, float gapRatio = 0.90f);

#endif // APP_H
