#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string_view>
#include <memory>

#include "Common/Exception.h"

class Label {
public:
    Label(SDL_Renderer* renderer)
    : renderer_(renderer) {
    }

    void setText(std::string_view text) {
        text_ = text;
    }

    void setColor(const SDL_Color& color) {
        color_ = color;
    }

    int getWidth() {
        return textureRect_.w;
    }

    int getHeight() {
        return textureRect_.h;
    }

    void setPos(int x, int y) {
        textureRect_.x = x;
        textureRect_.y = y;
    }

    void setFont(std::string_view font, int size) {
        TTF_Font* fontRaw = TTF_OpenFont(font.data(), size);
        if (!fontRaw)
            throw Exception(TTF_GetError());
        font_.reset(fontRaw);
    }

    SDL_Rect getRect() { return textureRect_; }

    void update() {
        SDL_Surface* surface =
            TTF_RenderText_Blended_Wrapped(font_.get(), text_.c_str(), color_, 0); 
        texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_FreeSurface(surface);
        // Update label width and height
        SDL_QueryTexture(texture_, nullptr, nullptr, &textureRect_.w, &textureRect_.h);
    }

    void draw() {
        SDL_RenderCopy(renderer_, texture_, NULL, &textureRect_);
    }

    ~Label() {

    }
private:
    SDL_Rect textureRect_ = {0, 0, 0, 0};
    using FontPtr = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;
    FontPtr font_{nullptr, &TTF_CloseFont};
    std::string text_;
    SDL_Color color_;
    SDL_Texture* texture_;
    SDL_Renderer* renderer_;
};



#endif //LABEL_H
