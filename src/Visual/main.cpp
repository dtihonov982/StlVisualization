#include "App.h"
#include <string>
#include <iostream>
#include <string_view>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: visual fps file1 file2 ... fileN\n";
        return 0;
    }
    int fps = std::stoi(argv[1]);

    std::vector<std::string_view> filenames(argv + 2, argv + argc);
    App app(filenames);

    const int frameDelay = 1000 / fps;
    int frameStart;
    int frameTime;
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
