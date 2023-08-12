#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <string>
#include "Chart.h"
#include "Script.h"
#include <stack>

class Player {
public:
    enum Status { Play, Pause, Done };
    Player(const SDL_Rect& geom, 
                   const std::string& title, 
                   const std::vector<int>& data, 
                   const Script& script);
    void update();
    void draw(SDL_Renderer* renderer);
    void toggleStatus();
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
private:
    std::string title_;
    std::vector<int> data_;
    std::stack<int> markedPos_;
    Script script_;
    typename Script::iterator currentAction_;
    Chart chart_;
    //Runs paused. Starts with Space.
    Status status_ = Pause;
};
#endif // PLAYER_H
