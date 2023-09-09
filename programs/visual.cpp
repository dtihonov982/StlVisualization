#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <stdexcept>

#include "Common/Config.h"
#include "Visual/App.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: visual delay_ratio file1 file2 ... fileN\n";
        return 1;
    }
    float delayRatio = 100.0f;
    try {
        delayRatio = std::stof(argv[1]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Incorrect delay ratio: " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string_view> filenames(argv + 2, argv + argc);

    Config config{"config.txt"};

    try {
        App app(config);
        app.createPlayers(delayRatio, filenames);
        app.run();
    }
    catch (const Exception& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
