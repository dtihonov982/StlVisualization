#include "Player.h"
#include <fstream>
#include "Exception.h"


Player Player::makePlayer(const SDL_Rect& rect, std::string_view filename) {
    std::ifstream file{filename.data()};
    if (!file)
        throw Exception("Can not open file: ", filename);

    std::string title;
    if(!std::getline(file, title))
        throw Exception("Title reading error.");

    std::string dump;
    if(!std::getline(file, dump))
        throw Exception("Data reading error.");


    std::vector<int> data; 
    try { 
        data = loadDataFromDump(dump, ',');
    }
    catch (const Exception& ex) {
        std::cerr << "Data parsing error.";
        throw ex;
    }

    Script script;
    try {
        script = readScript(file, ',');
    }
    catch (const Exception& ex) {
        std::cerr << "Can not create script from file " << filename << "\n";
        throw ex;
    }

    return Player(rect, title, data, script);
}

Player::Player(const SDL_Rect& geom, 
               const std::string& title, 
               const std::vector<int>& data, 
               const Script& script)
: title_(title)
, data_(data)
, script_(script) 
{
    chart_.setGeometry(geom);
    chart_.update(data_.begin(), data_.end());
}

void Player::update() {
    if (status_ == Pause || status_ == Done)
        return;

    //back colors to default
    while (!markedPos_.empty()) {
        chart_.resetElementColor(markedPos_.top());
        markedPos_.pop();
    }

    Action action = script_[currScriptPos_];
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

    ++currScriptPos_;

    if (currScriptPos_ >= script_.size())
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
