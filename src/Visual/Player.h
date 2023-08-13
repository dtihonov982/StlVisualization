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
    static Player makePlayer(const SDL_Rect& rect, std::string_view filename);
private:
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
