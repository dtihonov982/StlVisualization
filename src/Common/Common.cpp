#include <random>
#include <vector>
#include <cmath>

#include "Common.h"

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

SDL_Color toSDLColor(Color color) noexcept {
    SDL_Color res;
    res.r = (color >> 24) & 0xff;
    res.g = (color >> 16) & 0xff;
    res.b = (color >> 8)  & 0xff;
    res.a = color         & 0xff;
    return res;
}

SDL_Color getSDLColorFromConfig(const Config& config, const std::string& key, Color defaultColor) {

    auto colorRaw = config.get<Color>(key, defaultColor);
    return toSDLColor(colorRaw);
}
