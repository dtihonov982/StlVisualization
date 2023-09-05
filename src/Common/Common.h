#ifndef COMMON_H
#define COMMON_H

#include <random>
#include <iostream>
#include <vector>
#include <string_view>
#include <list>

#include <SDL2/SDL.h> 
#include "Config.h"

std::vector<int> getRandVector(int size, int min, int max);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    if (vec.empty()) 
        return os;

    os << vec[0];
    
    for (auto it = vec.cbegin() + 1; it != vec.cend(); ++it) {
        os << "," << *it;
    }
    return os;
}

using Color = long long;
SDL_Color toSDLColor(Color color) noexcept;
SDL_Color getSDLColorFromConfig(const Config& config, const std::string& key, Color defaultColor);

SDL_Rect roundRect(float x, float y, float w, float h);
#endif // COMMON_H
