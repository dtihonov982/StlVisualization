#include "Common.h"

#include <random>
#include <vector>
#include <cmath>

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

SDL_Rect roundRect(float x, float y, float w, float h) {
    return {static_cast<int>(x), static_cast<int>(y),
            static_cast<int>(w), static_cast<int>(h) };
}

