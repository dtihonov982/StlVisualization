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
    journal_ = readJournal(file , ',');
    currentEntry_ = journal_.begin();
    
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
    while (!markedPos_.empty()) {
        chart_.resetElementColor(markedPos_.top());
        markedPos_.pop();
    }

    auto entry = *currentEntry_;
    //TODO:double condition checking
    //TODO: many changes at one time for fast displaying of long arrays
    if (entry[0] == "access") {
        //std::cout << "reading at " << entry[1] << " position\n";
        int pos = std::stoi(entry[1]);
        chart_.setElementColor(pos, {255, 255, 255, 255});
        markedPos_.push(pos);
    }
    else if (entry[0] == "write") {
        //std::cout << "writing a " << entry[2] << " in " << entry[1] << " position\n";
        //TODO: doubling code
        int pos = std::stoi(entry[1]);
        int value = std::stoi(entry[2]);
        data_[pos] = value;
        chart_.update(data_.begin(), data_.end());

        chart_.setElementColor(pos, {255, 255, 255, 255});
        markedPos_.push(pos);
    }

    ++currentEntry_;

    if (currentEntry_ >= journal_.end())
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
