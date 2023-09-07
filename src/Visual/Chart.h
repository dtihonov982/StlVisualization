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
#include "Visual/ColoredSeries.h"

//draw chart from vector values
// The Chart represents a diapason [first, last) as a set of rectangles with different heights. 
class Chart {
public:
    Chart(const std::shared_ptr<Config>& config): config_(config) {
        auto defaultElementColor = getSDLColorFromConfig(*config_, "ElementsColor", 0xffffffff);
        series_.setDefaultColor(defaultElementColor);
    }

    //rect - area of the chart on screen. [begin; end) - data to show in chart
    void setGeometry(const SDL_Rect& rect) {
        series_.setGeometry(rect);
    }

    void draw(SDL_Renderer* renderer) {
        auto rectangles = series_.getElements();
        auto colors = series_.getColors();
        for (size_t i = 0; i < series_.size(); ++i) {
            auto rect = rectangles[i];
            auto color = colors[i];
            SDL_SetRenderDrawColor(
                renderer,
                color.r, 
                color.g,
                color.b,
                color.a
            );

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    template <typename It>
    void update(It begin, It end) {
        series_.load(begin, end);
    }

    void setElementColor(size_t pos, const SDL_Color& color) {
        series_.setElementColor(pos, color);
    }

    void resetElementColor(size_t pos) {
        series_.setColorsToDefault(pos);
    }

private:

    ColoredSeries series_;
    std::shared_ptr<Config> config_;
};

#endif //CHART_H
