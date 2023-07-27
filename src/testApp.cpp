#include "App.h"
#include <string>

int main(int argc, char** argv) {
    if (argc < 2)
        return 0;
    //TODO: CLI argument file to read
    App app(argv[1]);
    int fps;
    if (argc > 2) {
        fps = std::stoi(argv[2]);
    }
    else {
        fps = 25;
    }
    const int frameDelay = 1000 / fps;
    Uint32 frameStart;
    Uint32 frameTime;

    int x;
    std::cin >> x;

    while (app.isRunning()) {
        frameStart = SDL_GetTicks();
        
        app.handleEvents();
        app.update();
        app.render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }

    return 0;
}
