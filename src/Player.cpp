#include "Player.h"

Player::Player(SDL_Rect& geom, const std::vector<int>& data, const Script& script) 
: data_(data) 
, script_(script) {
    chart_.setGeometry(rect);
    chart_.update(data_.begin(), data_.end());
}
