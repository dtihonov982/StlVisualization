#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string_view>
#include <memory>

#include "Common/Exception.h"

class Label {
public:
    Label(SDL_Renderer* renderer);
    void setText(std::string_view text);
    void setColor(const SDL_Color& color);
    int getWidth();
    int getHeight();
    void setPos(int x, int y);
    void setFont(std::string_view font, int size);
    SDL_Rect getRect() { return textureRect_; }
    void update();
    void draw();
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
