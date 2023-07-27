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
    window_ = SDL_CreateWindow(windowTitle.c_str(), 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);

    isRunning_ = true;
}

void App::update() {
    if (currentEntry_ >= journal_.end())
        return;

    //back colors to default
    while (!markedPos_.empty()) {
        chart_.resetElementColor(markedPos_.top());
        markedPos_.pop();
    }

    auto entry = *currentEntry_;
    //TODO:double condition checking
    //TODO: many changes at one time for fast displaying of long arrays
    if (entry[0] == "r") {
        //std::cout << "reading at " << entry[1] << " position\n";
        int pos = std::stoi(entry[1]);
        chart_.setElementColor(pos, {255, 255, 255, 255});
        markedPos_.push(pos);
    }
    else if (entry[0] == "w") {
        //std::cout << "writing a " << entry[2] << " in " << entry[1] << " position\n";
        //TODO: doubling code
        int pos = std::stoi(entry[1]);
        int value = std::stoi(entry[2]);
        data_[pos] = value;
        //TODO:incremental
        chart_.update(data_.begin(), data_.end());

        chart_.setElementColor(pos, {255, 0, 0, 255});
        markedPos_.push(pos);
    }

    ++currentEntry_;
}

void App::handleEvents() {
    SDL_Event event;
    //TODO: start with space key
    while (SDL_PollEvent(&event)) {
        //event.type == SDL_KEYDOWN && event.key.keysum.sym == SDLK_ESCAPE
        if (event.type == SDL_QUIT)
            isRunning_ = false;
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
