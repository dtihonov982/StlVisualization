#ifndef CHART_H
#define CHART_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <SDL2/SDL.h>

#include "Common.h"

//return rectangles what fills area and represents [first; last) as a chart diagram
template<typename It>
std::vector<SDL_Rect> emplace(SDL_Rect area, It first, It last, float gapRate, int maxSize, int maxValue) {
    size_t size = last - first;
    assert(size <= maxSize);
    std::vector<SDL_Rect> result;
    if (size == 0)
        return result;
    if (size == 1)
        return result;
    if (maxSize > area.w)
        return result; //to many data to visualize in area

    //use float not integer to avoid from rounding routine in emplace logic
    float rectWidth = area.w / (maxSize + (maxSize - 1) * gapRate);     
    float gapWidth;
    if (rectWidth >= 1.0f) {
        gapWidth  = gapRate * rectWidth;
    }
    //If too many data for gape rate, change gape rate
    else {
        rectWidth = 1.0f;
        gapWidth = static_cast<float>(area.w - maxSize) / (maxSize - 1);
    }

    //SDL requires coordinates for top left corner of each rectangle
    //for computations use down left coordinates (base)
    float baseX = area.x;
    float baseY = area.y + area.h;

    while (first < last) {
        assert(*first <= maxValue);
        float currH = *first * area.h / maxValue;
        //after evaluations coordinates transfers from base to top-left
        SDL_Rect currRect = roundRect(baseX, baseY - currH, rectWidth, currH);
        result.push_back(currRect);
        baseX += gapWidth + rectWidth;

        ++first;
    }

    return result;
}

//TODO: draw method for element
//TODO: SDL_Rect instead x, y, ...
//TODO: set geometry after update
//draw chart from vector values
class Chart {
public:
    Chart() {}
    //rect - area of the chart on screen. [begin; end) - data to show in chart
    template <typename It>
    Chart(const SDL_Rect& rect, It begin, It end)
    : x_(rect.x), y_(rect.y), w_(rect.w), h_(rect.h)
    {   
        update(begin, end);
    }

    void setGeometry(const SDL_Rect& rect) {
        x_ = rect.x; 
        y_ = rect.y; 
        w_ = rect.w; 
        h_ = rect.h;
    }

    void dump() {
        std::cout << "x: " << x_ << '\n';
        std::cout << "y: " << y_ << '\n';
        std::cout << "w: " << w_ << '\n';
        std::cout << "h: " << h_ << '\n';
        std::cout << "gap rate: " << gapRate_ << '\n';

        for (auto& element: elements_) {
            std::cout << "(x: " << element.x_ << ", y: ";
            std::cout << element.y_  << ", w: ";
            std::cout << element.w_  << ", h: ";
            std::cout << element.h_ << ")\n";
        }

        if (!elements_.empty()){
            auto last = elements_.back();
            std::cout << "bound: " << last.x_ + last.w_ << '\n';
        }
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(
            renderer,
            backgroundColor_.r, 
            backgroundColor_.g,
            backgroundColor_.b,
            backgroundColor_.a
        );
        SDL_Rect chartArea {x_, y_, w_, h_};

        SDL_RenderFillRect(renderer, &chartArea);

        for (auto& element: elements_) {
            SDL_SetRenderDrawColor(
                renderer,
                element.color_.r, 
                element.color_.g,
                element.color_.b,
                element.color_.a
            );

            SDL_Rect current {element.x_, element.y_, element.w_, element.h_};
            SDL_RenderFillRect(renderer, &current);
        }
    }

    template <typename It>
    void update(It begin, It end) {
        elements_.clear();

        if (end - begin <= 0)
            return;

        SDL_Rect area{x_, y_, w_, h_};
        int max = *std::max_element(begin, end);
        auto rectangles = emplace(area, begin, end, gapRate_, end - begin, max);

        for (auto& rect: rectangles) {
            elements_.emplace_back(rect.x, rect.y, rect.w, rect.h, defaultElementColor_);
        }
    }

    void setElementColor(int pos, const SDL_Color& color) {
        elements_[pos].color_ = color;
    }

    void resetElementColor(int pos) {
        elements_[pos].color_ = defaultElementColor_;
    }

    void setDefaultElementColor(const SDL_Color& color) {
        defaultElementColor_ = color;
    }

    void setBackgroundColor(const SDL_Color& color) {
        backgroundColor_ = color;
    }

    struct Element {
        Element(float x, float y, float w, float h, const SDL_Color& color):
        x_(static_cast<int>(x)),
        y_(static_cast<int>(y)),
        w_(static_cast<int>(w)),
        h_(static_cast<int>(h)),
        color_(color) {}
        
        int x_;
        int y_;
        int w_;
        int h_;
        SDL_Color color_;
    };
private:
    std::vector<Element> elements_;
    int x_;
    int y_;
    int w_;
    int h_;
    SDL_Color backgroundColor_     = {  0x00,   0x3f,   0x5c, 255};
    SDL_Color defaultElementColor_ = { 0xff, 0x63, 0x61, 255};
    float gapRate_ = 2.0f; //gapRate_ = gapWidth / elementWidth;
};

#endif //CHART_H
