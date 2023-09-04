#include "App.h"
#include "Common/Common.h"
#include <fstream>

using namespace std::chrono;
using namespace std::chrono_literals;


App::App(const Config& config)
: config_(std::make_shared<Config>(config)) {
    windowWidth_ = config_->get<int>("WindowWidth", 1024);
    windowHeight_ = config_->get<int>("WindowHeight", 640);
    Color backgroundColorRaw = config_->get<Color>("BackgroundColor", 0x00000000);
    backgroundColor_ = toSDLColor(backgroundColorRaw);
    initGraphics();
}

std::vector<SDL_Rect> getGrid(int cellsCount, int totalWidth, int totalHeight, float cellRatio = 0.90) {
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

void App::createPlayers(float delayRatio, const std::vector<std::string_view>& files) {
    players_.clear();
    size_t count = files.size();
    std::vector<SDL_Rect> blocks = getGrid(count, windowWidth_, windowHeight_);
    for (size_t i = 0; i < count; ++i) {
        try {
            Record record = Record::load(files[i]);
            players_.emplace_back(renderer_, blocks[i], 
                delayRatio, std::move(record), config_);
        }
        catch (const Exception& ex) {
            std::cerr << "Error while creating player for " << files[i] << "\n";
            throw ex;
        }
    }
}
    
void App::initGraphics() {
    TTF_Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw Exception("SDL initiation error.");
    }

    window_ = SDL_CreateWindow("Title", 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth_, windowHeight_, SDL_WINDOW_FULLSCREEN);
    if (!window_)
        throw Exception("Window creating error.");
    //SDL_SetWindowBordered(window_, SDL_FALSE);

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_)
        throw Exception("Renderer creating error.");

}

void App::run() {
    isRunning_ = true;
    initScheduler();
    while (isRunning()) {
        IEventHandlerPtr handler = sched_.wait();
        WakeUp event{&sched_};
        handler->handle(event);
        draw();
    }
}

void App::handle(Event& event) {
    if (event.getType() != Event::WakeUp) 
        return;
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT: isRunning_ = false; return;
            case SDL_KEYDOWN: handleKeyDown(sdlEvent); break;
        }
    }
    WakeUp& e = static_cast<WakeUp&>(event);
    e.sched->add(40ms, this);
}

void App::initScheduler() {
    sched_.clearTasks();
    for (auto& player: players_)
        sched_.add(player.getMsToNextAction(), &player);
    // TODO: ups as class member
    sched_.add(40ms, this);
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

void App::draw() {
    SDL_SetRenderDrawColor(renderer_, 
        backgroundColor_.r, backgroundColor_.g, 
        backgroundColor_.b, backgroundColor_.a);
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

