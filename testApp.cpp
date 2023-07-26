#include "App.h"

int main() {
    //TODO: CLI argument file to read
    App app("journal.txt");
    constexpr int fps = 5;
    constexpr int frameDelay = 1000 / fps;
    Uint32 frameStart;
    Uint32 frameTime;
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
