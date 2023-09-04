#ifndef CHART_H
#define CHART_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <memory>

#include <SDL2/SDL.h>

#include "Common/Common.h"
#include "Common/Config.h"
#include "Common/Exception.h"
#include "Visual/Rectangle.h"

//draw chart from vector values
// The Chart represents a diapason [first, last) as a set of rectangles with different heights. 
class Chart {
public:
    Chart(const std::shared_ptr<Config>& config): config_(config) {
        Color backgroundColorRaw = config_->get<Color>("BackgroundColor", 0x00000000);
        background_.color = toSDLColor(backgroundColorRaw);
        Color defaultElementColorRaw = config_->get<Color>("ElementsColor", 0xffffffff);
        defaultElementColor_ = toSDLColor(defaultElementColorRaw);
    }

    //rect - area of the chart on screen. [begin; end) - data to show in chart
    void setGeometry(const SDL_Rect& rect) {
        background_.geom = rect;
    }

    void draw(SDL_Renderer* renderer) {
        ::draw(renderer, background_);
        for (auto& element: elements_) {
            ::draw(renderer, element);
        }
    }

    template <typename It>
    void update(It begin, It end) {
        elements_.clear();

        if (end - begin <= 0)
            return;

        int max = *std::max_element(begin, end);
        auto rectangles = emplace(background_.geom, begin, end, gapRate_, max);

        for (auto& rect: rectangles) {
            elements_.push_back({rect, defaultElementColor_});
        }
    }

    void setElementColor(size_t pos, const SDL_Color& color) {
        if (pos < elements_.size())
            elements_[pos].color = color;
        else 
            throw Exception("Invalid chart element position ", pos, ".There is ", elements_.size(), " elements in chart.");
    }

    void resetElementColor(size_t pos) {
        if (pos < elements_.size())
            elements_[pos].color = defaultElementColor_;
        else 
            throw Exception("Invalid chart element position ", pos, ".There is ", elements_.size(), " elements in chart.");
    }

    void setDefaultElementColor(const SDL_Color& color) {
        defaultElementColor_ = color;
    }

    void setBackgroundColor(const SDL_Color& color) {
        background_.color = color;
    }

private:
    //return rectangles what fills area and represents [first; last) as a chart diagram
    template<typename It>
    static std::vector<SDL_Rect> emplace(SDL_Rect area, It first, It last, float gapRate, int maxValue) {
        int size = last - first;
        assert(size <= size);
        std::vector<SDL_Rect> result;
        if (size == 0)
            return result;
        if (size == 1)
            return result;
        if (size > area.w)
            return result; //to many data to visualize in area

        //use a float not an integer to avoid rounding routine in emplace logic
        float rectWidth = area.w / (size + (size - 1) * gapRate);     
        float gapWidth;
        if (rectWidth >= 1.0f) {
            gapWidth  = gapRate * rectWidth;
        }
        //If too many data for gape rate, change gape rate
        else {
            rectWidth = 1.0f;
            gapWidth = static_cast<float>(area.w - size) / (size - 1);
        }

        //SDL requires coordinates for top left corner of each rectangle
        //for computations use down left coordinates (base)
        float baseX = area.x;
        float baseY = area.y + area.h;

        while (first < last) {
            int currValue = *first;
            assert(currValue <= maxValue);
            SDL_Rect currRect;
            if (currValue != 0) {
                float currH = currValue * area.h / maxValue;
                //after evaluations coordinates transfers from base to top-left
                currRect = roundRect(baseX, baseY - currH, rectWidth, currH);
            }
            else {
                currRect = roundRect(baseX, baseY, rectWidth, 0.0f);
            }
            result.push_back(currRect);

            baseX += gapWidth + rectWidth;
            ++first;
        }

        return result;
    }

    std::vector<Rectangle> elements_;
    Rectangle background_{0, 0, 0, 0, {  0x00,   0x3f,   0x5c, 255}};
    SDL_Color defaultElementColor_ = { 0xff, 0x63, 0x61, 255};
    std::shared_ptr<Config> config_;
    float gapRate_ = 2.0f; //gapRate_ = gapWidth / elementWidth;
};

#endif //CHART_H
