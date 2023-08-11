#include "Chart.h"

int main() {
    SDL_Rect area{100, 100, 100, 100};
    std::vector<int> data{101, 201, 301};
    auto result = emplace(area, data.begin(), data.end(), 2, 301);
    return 0;
}
