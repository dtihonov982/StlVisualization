#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

#include <SDL2/SDL.h>

#include "Common/Common.h"
#include "Common/Exception.h"

// Represents numbers as rectangles.
class Series {
public:
    
    void setGeom(const SDL_Rect& geom) {
        geom_ = geom;
    }

    void setGapRate(float rate) {
        gapRate_ = rate;
    }

    const std::vector<SDL_Rect>& getElements() const {
        return elements_;
    }
    
    template<typename It>
    void load(It begin, It end) {
        elements_.clear();

        if (end - begin <= 0)
            return;

        int max = *std::max_element(begin, end);
        elements_ = emplace(geom_, begin, end, gapRate_, max);
    }

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
    virtual ~Series() {}
private:
    std::vector<SDL_Rect> elements_;
    SDL_Rect geom_ = {0, 0, 100, 100};
    float gapRate_ = 2.0f;
};
