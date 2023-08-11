#include "Common.h"

#include <random>
#include <vector>

std::vector<int> getRandVector(int size, int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    std::vector<int> res;
    res.reserve(size);
    for (int i = 0; i < size; ++i) {
        res.push_back(dist(rng));
    }
    return res;
}



#define src/CHART_H


SDL_Rect roundRect(float x, float y, float w, float h) {
    return {static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(w), static_cast<float>(h) };
}

