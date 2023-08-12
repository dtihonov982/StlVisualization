#include "App.h"
#include <fstream>


App::App(std::string_view logfile) {
    std::ifstream file{logfile.data()};
    if (!file.is_open()) {
        return;
    }
    std::getline(file, title_);
    std::string dump;
    std::getline(file, dump);
    data_ = loadDataFromDump(dump, ',');
    script_ = readScript(file , ',');
    currentAction_ = script_.begin();
    
    std::string windowTitle = "STL visualization";
    int windowWidth = 800;
    int windowHeight = 640;

    float chartAreaRatio = 0.80f;
    int chartWidth = static_cast<int>(windowWidth * chartAreaRatio);
    int chartHeight = static_cast<int>(windowHeight * chartAreaRatio);
    int chartX = (windowWidth - chartWidth) / 2;
    int chartY = (windowHeight - chartHeight) / 2;
    SDL_Rect rect {chartX, chartY, chartWidth, chartHeight};

    chart_.setGeometry(rect);
    chart_.update(data_.begin(), data_.end());

    
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = SDL_CreateWindow(title_.data(), 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);

    isRunning_ = true;
}

void App::update() {
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

void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: isRunning_ = false; break;
            case SDL_KEYDOWN: handleKeyDown(event); break;
        }
    }
}

void App::handleKeyDown(SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE: 
        isRunning_ = false; 
        break;
    case SDLK_SPACE: 
        if (status_ == Done) {
            break;
        }
        else if (status_ == Play) {
            status_ = Pause;
            break;
        }
        else if (status_ == Pause) {
            status_ = Play;
            break;
        }
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer_, 0x00,   0x3f,   0x5c, 255);
    SDL_RenderClear(renderer_);

    chart_.draw(renderer_);

    SDL_RenderPresent(renderer_);
}

App::~App() {
    //TODO: check if objects have created
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}
