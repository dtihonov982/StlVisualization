#include <fstream>
#include "Common/Exception.h"
#include "Visual/Player.h"

using namespace std::chrono;

Player Player::makePlayer(SDL_Renderer* renderer, const SDL_Rect& rect, uint64_t delayRatio, std::string_view filename) {
    Record record = Record::load(filename);
    return Player(renderer, rect, delayRatio, record.info, record.data, record.script);
}

Player::Player(SDL_Renderer* renderer,
               const SDL_Rect& area, 
               uint64_t delayRatio,
               const std::string& title, 
               const std::vector<int>& data, 
               const Script& script)
: renderer_(renderer)
, delayRatio_(delayRatio)
, title_(title)
, data_(data)
, script_(script) 
, label_(renderer_)
{
    initLabel();
    arrangeElements(area);
    chart_.update(data_.begin(), data_.end());
}

void Player::arrangeElements(const SDL_Rect& area) {
    int labelWidth = label_.getWidth();
    int labelHeight = label_.getHeight();

    int chartAreaHeight = area.h - labelHeight - chartLabelSpace;
    SDL_Rect chartArea {area.x, area.y, area.w, chartAreaHeight};

    int labelX = area.x + (area.w - labelWidth) / 2;
    int labelY = (area.y + area.h - labelHeight);
    label_.setPos(labelX, labelY);

    chart_.setGeometry(chartArea);
}

void Player::initLabel() {
    label_.setText(title_);
    label_.setFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 24);
    label_.setColor({0xff, 0xff, 0xff, 0xff});
    label_.update();
}

void Player::handle(Event& event) {
    if (status_ == Done)
        return;

    if (event.getType() != Event::WakeUp) 
        return;
    WakeUp& e = static_cast<WakeUp&>(event);

    if (status_ != Pause) {
        dropMarkedElements();

        Action action = script_[currScriptPos_];
        handleAction(action);

        //draw();

        ++currScriptPos_;

        if (currScriptPos_ >= script_.size()) {
            status_ = Done;
            return;
        }
    }

    e.sched->add(getMsToNextAction(), this);
}
void Player::dropMarkedElements() {
    //back colors to default
    while (!markedPos_.empty()) {
        chart_.resetElementColor(markedPos_.top());
        markedPos_.pop();
    }
}

void Player::handleAction(const Action& action) {
    switch (action.type) {
    case Action::ACCESS:
        chart_.setElementColor(action.pos, {255, 255, 255, 255});
        markedPos_.push(action.pos);
        break;
    case Action::WRITE:
        data_[action.pos] = action.value;
        chart_.update(data_.begin(), data_.end());

        chart_.setElementColor(action.pos, {255, 255, 255, 255});
        markedPos_.push(action.pos);
        break;
    default:
        throw Exception("Unknown Action type.");
    }
}

std::chrono::milliseconds Player::getMsToNextAction() const {
    assert(currScriptPos_ < script_.size());
    const Action& curr = script_[currScriptPos_];
    uint64_t rawDelay = curr.timePoint;
    if (currScriptPos_ > 0) {
        const Action& prev = script_[currScriptPos_ - 1];
        rawDelay -= prev.timePoint;
    }
    return milliseconds(rawDelay / delayRatio_);
}

void Player::toggleStatus() {
    if (status_ == Play) {
        status_ = Pause;
    }
    else if (status_ == Pause) {
        status_ = Play;
    }
}

void Player::draw() {
    chart_.draw(renderer_);
    label_.draw();
}

