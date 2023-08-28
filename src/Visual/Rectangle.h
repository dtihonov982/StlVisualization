#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>

struct Rectangle {
    SDL_Rect  geom;
    SDL_Color color;
};

void draw(SDL_Renderer* renderer, const Rectangle& rect);

#endif // RECTANGLE_H
