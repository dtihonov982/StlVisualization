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

class Player: public IEventHandler {
public:
    enum Status { Play, Pause, Done };
    Player(const SDL_Rect& geom, 
           uint64_t delayRatio,
           const std::string& title, 
           const std::vector<int>& data, 
           const Script& script);
    void update();
    void draw(SDL_Renderer* renderer);
    void toggleStatus();
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
    static Player makePlayer(const SDL_Rect& rect, uint64_t delayRatio, std::string_view filename);
    void handleAction(const Action& action);
    uint64_t getMsToNextAction() const;
    void handle(Event& event) override;
private:
    uint64_t delayRatio_;
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
