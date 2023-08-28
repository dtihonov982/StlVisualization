#include "Visual/Rectangle.h"

void draw(SDL_Renderer* renderer, const Rectangle& rect) {
    SDL_SetRenderDrawColor(
        renderer,
        rect.color.r, 
        rect.color.g,
        rect.color.b,
        rect.color.a
    );

    SDL_RenderFillRect(renderer, &rect.geom);
}
