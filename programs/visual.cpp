#include "Visual/App.h"
#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: visual delay_ratio file1 file2 ... fileN\n";
        return 1;
    }
    int delayRatio = 100;
    try {
        delayRatio = std::stoi(argv[1]);
    }
    catch (const std::invalid_argument& ex) {
        std::cerr << "Incorrect delay ratio: " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string_view> filenames(argv + 2, argv + argc);

    try {
        App app(delayRatio, filenames);
        app.run();
    }
    catch (const Exception& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    return 0;
}
