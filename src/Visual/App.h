#ifndef APP_H
#define APP_H

#include <string_view>
#include <string>
#include <stack>
#include <cstdint>
#include <chrono>

#include <SDL2/SDL.h>

#include "Common/Config.h"
#include "Common/Exception.h"
#include "Common/Record.h"
#include "Logger/Event.h"
#include "Visual/Chart.h"
#include "Visual/Scheduler.h"
#include "Visual/Player.h"

class App: public IEventHandler {
public:
    App(float delayRatio, const std::vector<std::string_view>& files, const Config& config);
    ~App();
    void run();
private:
    void initScheduler();
    void initGraphics();
    void createPlayers(float delayRatio, const std::vector<std::string_view>& files);

    // On each event the App checks if a key was press.
    void handle(Event& event) override;
    void handleKeyDown(SDL_Event& event);

    void draw();

    bool isRunning() { return isRunning_; }

    std::shared_ptr<Config> config_;

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Color backgroundColor_;
    int windowWidth_;
    int windowHeight_;

    Scheduler<IEventHandlerPtr> sched_;
    std::vector<Player> players_;

    bool isRunning_ = false;
};

#endif // APP_H
