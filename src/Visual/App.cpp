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

App::App(const std::vector<std::string_view>& files) {
    
    int windowWidth = 1024;
    int windowHeight = 640;

    size_t count = files.size();
    std::vector<SDL_Rect> blocks = emplaceBlocks(count, windowWidth, windowHeight);
    for (size_t i = 0; i < count; ++i) {
        try {
            Player curr = Player::makePlayer(blocks[i], files[i]);
            players_.push_back(std::move(curr));
        }
        catch (const Exception& ex) {
            std::cerr << "Error while creating player for " << files[i] << "\n";
            throw ex;
        }
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw Exception("SDL initiation error.");
    }

    window_ = SDL_CreateWindow("Title", 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth, windowHeight, 0);
    if (!window_)
        throw Exception("Window creating error.");

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_)
        throw Exception("Renderer creating error.");

    isRunning_ = true;
}


void App::run(int frameDelay) {
    while (isRunning()) {
        int frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }
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
