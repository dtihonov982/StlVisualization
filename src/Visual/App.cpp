#include "App.h"
#include <fstream>


std::vector<SDL_Rect> App::emplaceBlocks(int count, int width, int height, float gapRatio) {
    std::vector<SDL_Rect> blocks;
    int blockWidth = width * gapRatio;
    int gap = (width - blockWidth) / 2;
    int blockHeight = (height - gap * (count + 1)) / count;
    for (int i = 0; i < count; ++i) {
        SDL_Rect curr;
        curr.x = gap;
        curr.y = gap + i * (blockHeight + gap);
        curr.w = blockWidth;
        curr.h = blockHeight;
        blocks.push_back(curr);
    }
    return blocks;
}

App::App(std::string_view logfile) {
    std::ifstream file{logfile.data()};
    if (!file.is_open()) {
        return;
    }
    std::string title;
    std::getline(file, title);

    std::string dump;
    std::getline(file, dump);
    std::vector<int> data = loadDataFromDump(dump, ',');

    Script script = readScript(file , ',');
    
    int windowWidth = 800;
    int windowHeight = 640;
    float chartAreaRatio = 0.80f;

    int chartWidth = static_cast<int>(windowWidth * chartAreaRatio);
    int chartHeight = static_cast<int>(windowHeight * chartAreaRatio);
    int chartX = (windowWidth - chartWidth) / 2;
    int chartY = (windowHeight - chartHeight) / 2;
    SDL_Rect rect {chartX, chartY, chartWidth, chartHeight};

    players_.emplace_back(rect, title, data, script);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window_ = SDL_CreateWindow(title.data(), 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);

    isRunning_ = true;
}

void App::update() {
    for (auto& player: players_)
        player.update();
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
        for (auto& player: players_)
            player.toggleStatus();
        break;
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer_, 0x00,   0x3f,   0x5c, 255);
    SDL_RenderClear(renderer_);

    for (auto& player: players_)
        player.draw(renderer_);

    SDL_RenderPresent(renderer_);
}

App::~App() {
    //TODO: check if objects have created
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}
