#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string_view>
#include "Exception.h"

class Label {
public:
    Label(SDL_Renderer* renderer,
          int x,
          int y, 
          std::string_view text, 
          std::string_view font,
          int size,
          const SDL_Color& color)
    : renderer_(renderer) {
        texture_ = createTexture(renderer, text, font, size, color);
        textureRect_ = getTextureRect(x, y, texture_);
    }


    void draw() {
        SDL_RenderCopy(renderer_, texture_, NULL, &textureRect_);
    }

    static SDL_Texture* createTexture(SDL_Renderer* renderer,
                                     std::string_view text, 
                                     std::string_view font,
                                     int size, 
                                     const SDL_Color& color) {
        TTF_Font* ttfFont = TTF_OpenFont(font.data(), size);
        if (!ttfFont)
            throw Exception(TTF_GetError());
        SDL_Surface* surface =
            TTF_RenderText_Blended_Wrapped(ttfFont, text.data(), color, 0); 
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        TTF_CloseFont(ttfFont);
        SDL_FreeSurface(surface);
        return texture;
    }

    static SDL_Rect getTextureRect(int x, int y, SDL_Texture* texture) {
        SDL_Rect result{x, y, 0, 0};
        SDL_QueryTexture(texture, nullptr, nullptr, &result.w, &result.h);
        return result;
    }

    ~Label() {

    }
private:
    SDL_Rect textureRect_;
    SDL_Texture* texture_;
    SDL_Renderer* renderer_;
};



#endif //LABEL_H
