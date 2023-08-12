#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(SDL_Rect& geom, const std::vector<int>& data, const Script& script) 
    : data_(data) 
    , script_(script) {
    chart_.setGeometry(rect);
    chart_.update(data_.begin(), data_.end());
    }
    Status getStatus() { return status_;  }
    void setStatus(Status status) { status_ = status; }
    void update();
    void render(SDL_Renderer* renderer);
private:
    std::string title_;
    Status status_ = Pause;
    std::vector<int> data_;
    Script script_;
    typename Script::iterator currentAction_;
    std::stack<int> markedPos_;
    Chart chart_;
}

#endif //PLAYER_H
