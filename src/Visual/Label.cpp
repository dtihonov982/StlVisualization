#include "Visual/Label.h"

Label::Label(SDL_Renderer* renderer)
: renderer_(renderer) {
}

void Label::setText(std::string_view text) {
    text_ = text;
}

void Label::setColor(const SDL_Color& color) {
    color_ = color;
}

int Label::getWidth() {
    return textureRect_.w;
}

int Label::getHeight() {
    return textureRect_.h;
}

void Label::setPos(int x, int y) {
    textureRect_.x = x;
    textureRect_.y = y;
}

void Label::setFont(std::string_view font, int size) {
    TTF_Font* fontRaw = TTF_OpenFont(font.data(), size);
    if (!fontRaw)
        throw Exception(TTF_GetError());
    font_.reset(fontRaw);
}

void Label::update() {
    SDL_Surface* surface =
        TTF_RenderText_Blended_Wrapped(font_.get(), text_.c_str(), color_, 0); 
    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    // Update label width and height
    SDL_QueryTexture(texture_, nullptr, nullptr, &textureRect_.w, &textureRect_.h);
}

void Label::draw() {
    SDL_RenderCopy(renderer_, texture_, NULL, &textureRect_);
}

// TODO: Destructor. Smart pointer for texture
