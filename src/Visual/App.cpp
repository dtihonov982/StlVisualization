#include "App.h"
#include <fstream>

using namespace std::chrono;
using namespace std::chrono_literals;


App::App(uint64_t delayRatio, const std::vector<std::string_view>& files) {
    windowWidth_ = 1024;
    windowHeight_ = 640;
    
    initGraphics();
    createPlayers(delayRatio, files);
}

void App::createPlayers(uint64_t delayRatio, const std::vector<std::string_view>& files) {
    size_t count = files.size();
    std::vector<SDL_Rect> blocks = getGrid(count, windowWidth_, windowHeight_);
    for (size_t i = 0; i < count; ++i) {
        try {
            Player curr = Player::makePlayer(renderer_, blocks[i], delayRatio, files[i]);
            players_.push_back(std::move(curr));
        }
        catch (const Exception& ex) {
            std::cerr << "Error while creating player for " << files[i] << "\n";
            throw ex;
        }
    }
}
    
std::vector<SDL_Rect> getGrid(int cellsCount, int totalWidth, int totalHeight, float cellRatio) {
    std::vector<SDL_Rect> blocks;
    int blockWidth = totalWidth * cellRatio;
    int gap = (totalWidth - blockWidth) / 2;
    int blockHeight = (totalHeight - gap * (cellsCount + 1)) / cellsCount;
    for (int i = 0; i < cellsCount; ++i) {
        SDL_Rect curr;
        curr.x = gap;
        curr.y = gap + i * (blockHeight + gap);
        curr.w = blockWidth;
        curr.h = blockHeight;
        blocks.push_back(curr);
    }
    return blocks;
}

void App::initGraphics() {
    TTF_Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw Exception("SDL initiation error.");
    }

    window_ = SDL_CreateWindow("Title", 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth_, windowHeight_, 0);
    if (!window_)
        throw Exception("Window creating error.");

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_)
        throw Exception("Renderer creating error.");

    isRunning_ = true;
}

void App::run() {
    initScheduler();
    while (isRunning()) {
        IEventHandlerPtr handler = sched_.wait();
        WakeUp event{&sched_};
        handler->handle(event);
        render();
    }
}

void App::handle(Event& event) {
    if (event.getType() != Event::WakeUp) 
        return;
    WakeUp& e = static_cast<WakeUp&>(event);
    handleEvents();
    e.sched->add(40ms, this);
}

void App::initScheduler() {
    for (auto& player: players_)
        sched_.add(player.getMsToNextAction(), &player);
    // TODO: ups as class member
    sched_.add(40ms, this);
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
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x3f, 0x5c, 0xff);
    SDL_RenderClear(renderer_);

    for (auto& player: players_)
        player.draw();

    SDL_RenderPresent(renderer_);
}

App::~App() {
    //TODO: check if objects have created
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

