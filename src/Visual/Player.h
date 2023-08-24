#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include <stack>
#include <cstdint>

#include "Visual/Chart.h"
#include "Common/Script.h"
#include "Logger/Event.h"
#include "Visual/Scheduler.h"
#include "Visual/Label.h"

class Player: public IEventHandler {
public:
    enum Status { Play, Pause, Done };
    Player(SDL_Renderer* renderer,
           const SDL_Rect& area, 
           uint64_t delayRatio,
           const std::string& title, 
           const std::vector<int>& data, 
           const Script& script);
    void draw();
    void toggleStatus();
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
    static Player makePlayer(SDL_Renderer* renderer, const SDL_Rect& rect, uint64_t delayRatio, std::string_view filename);
    void handle(Event& event) override;
    std::chrono::milliseconds getMsToNextAction() const;
private:
    void handleAction(const Action& action);

    void initLabel();
    void arrangeElements(const SDL_Rect& area);

    SDL_Renderer* renderer_;
    Label label_;
    static constexpr int chartLabelSpace = 20;

    uint64_t delayRatio_;
    void dropMarkedElements();
    std::string title_;
    std::vector<int> data_;
    std::stack<int> markedPos_;
    Script script_;
    size_t currScriptPos_ = 0;
    Chart chart_;
    //Runs paused. Starts with Space.
    Status status_ = Pause;
};
#endif // PLAYER_H
