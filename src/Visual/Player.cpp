#include "Player.h"
#include <fstream>


Player::Player(const SDL_Rect& geom, 
               const std::string& title, 
               const std::vector<int>& data, 
               const Script& script)
: title_(title)
, data_(data)
, script_(script) 
, currentAction_(script_.begin())
{
    chart_.setGeometry(geom);
    chart_.update(data_.begin(), data_.end());
}

void Player::update() {
    if (status_ == Pause || status_ == Done)
        return;

    //back colors to default
    //TODO: do not drop marked before writing?
    while (!markedPos_.empty()) {
        chart_.resetElementColor(markedPos_.top());
        markedPos_.pop();
    }

    auto action = *currentAction_;
    //TODO: many changes at one time for fast displaying of long arrays
    if (action.type == Action::ACCESS) {
        chart_.setElementColor(action.pos, {255, 255, 255, 255});
        markedPos_.push(action.pos);
    }
    else if (action.type == Action::WRITE) {
        data_[action.pos] = action.value;
        chart_.update(data_.begin(), data_.end());

        chart_.setElementColor(action.pos, {255, 255, 255, 255});
        markedPos_.push(action.pos);
    }

    ++currentAction_;

    if (currentAction_ >= script_.end())
        status_ = Done;
}

void Player::toggleStatus() {
    if (status_ == Play) {
        status_ = Pause;
    }
    else if (status_ == Pause) {
        status_ = Play;
    }
}

void Player::draw(SDL_Renderer* renderer) {
    chart_.draw(renderer);
}
