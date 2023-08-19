#include "Visual/App.h"
#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: visual fps file1 file2 ... fileN\n";
        return 1;
    }
    int fps = 25;
    try {
        fps = std::stoi(argv[1]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Incorrect fps: " << argv[1] << "\n";
        return 1;
    }

    const int frameDelay = 1000 / fps;
    std::vector<std::string_view> filenames(argv + 2, argv + argc);

    try {
        App app(filenames);
        while (app.isRunning()) {
            int frameStart = SDL_GetTicks();

            app.handleEvents();
            app.update();
            app.render();

            int frameTime = SDL_GetTicks() - frameStart;

            if (frameTime < frameDelay)
                SDL_Delay(frameDelay - frameTime);
        }
    }
    catch (const Exception& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
