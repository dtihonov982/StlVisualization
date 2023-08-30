#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include <stack>
#include <cstdint>
#include <string_view>

#include "Visual/Chart.h"
#include "Common/Record.h"
#include "Logger/Event.h"
#include "Visual/Scheduler.h"
#include "Visual/Label.h"
#include "Common/Config.h"

class Player: public IEventHandler {
public:
    enum Status { Play, Pause, Done };
    Player(SDL_Renderer* renderer,
           const SDL_Rect& area, 
           float delayRatio,
           Record&& record,
           const std::shared_ptr<Config>& config);
    void draw();
    void toggleStatus();
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
    void handle(Event& event) override;
    std::chrono::milliseconds getMsToNextAction() const;
private:
    void handleAction(const Action& action);

    void initLabel(std::string_view title);
    void arrangeElements(const SDL_Rect& area);
    void dropMarkedElements();

    SDL_Renderer* renderer_;
    Label label_;
    int chartLabelSpace = 20;

    float delayRatio_;
    std::vector<int> data_;
    std::stack<int> markedPos_;
    Script script_;
    size_t currScriptPos_ = 0;
    Chart chart_;
    //Runs paused. Starts with Space.
    Status status_ = Pause;
    std::shared_ptr<Config> config_;
    SDL_Color accessColor_;
};
#endif // PLAYER_H
