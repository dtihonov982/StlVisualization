#include "App.h"


App::App(const Config& conf) {
    int width = conf.get<int>("WindowWidth");
    int height = conf.get<int>("WindowHeight");
    //TODO
    std::string title = conf.get<std::string>("WindowTitle");
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window_ = SDL_CreateWindow(title.c_str(), 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    width, height, 0);
    SDL_Renderer* renderer_ = SDL_CreateRenderer(window_, -1, 0);
    //TODO conf.get<int>("BacgroundRed") ...
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
}

void App::update() {
}

void App::render() {
	SDL_RenderClear(renderer_);

	SDL_RenderPresent(renderer_);
}

App::~App() {
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
	SDL_Quit();
}
