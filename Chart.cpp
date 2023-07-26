#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <numeric>
#include <vector>
#include <cassert>
#include <sstream>
#include <SDL2/SDL.h>


//draw chart from vector values
class Chart {
public:
    //rect - area of the chart on screen. [begin; end) - data to show in chart
    template <typename It>
    Chart(const SDL_Rect& rect, It begin, It end)
    : x_(rect.x), y_(rect.y), w_(rect.w), h_(rect.h)
    {   
        update(begin, end);
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
        int sz = end - begin;

        if (sz == 0)
            return;

        int max = *std::max_element(begin, end);

        if (sz == 1)
            return;
        
        if (sz > w_)
            return; //to many data
        

        float rectWidth = w_ / (sz + (sz - 1) * gapRate_ );     
        float gapWidth;
        //If too many data for gape rate, change gape rate
        if (rectWidth < 1.0f) {
            rectWidth = 1.0f;
            gapWidth = static_cast<float>(w_ - sz) / (sz - 1);
        }
        else {
            gapWidth  = gapRate_ * rectWidth;
        }

        std::cout << "rectangle width: " << rectWidth << '\n';
        std::cout << "gap width: " << gapWidth << '\n';
        float scale = static_cast<float>(h_) / max;
        std::cout << "scale: " << scale << '\n';

        float baseX = x_;
        float baseY = y_ + h_;

        float currentH = *begin * h_ / max;
        elements_.emplace_back(baseX, baseY - currentH, rectWidth, currentH, defaultElementColor_);

        for (auto it = begin + 1; it != end; ++it) {
            baseX += gapWidth + rectWidth;
            currentH = *it * h_ / max; //???
            elements_.emplace_back(baseX, baseY - currentH, rectWidth, currentH, defaultElementColor_);
        }

        assert(sz == elements_.size());
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


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const auto& x: vec) {
        os << x << ", ";
    }
    return os;
}

int main() {
    #if 0
    chart.draw();
    #endif
    int windowWidth = 800;
    int windowHeight = 640;
    const char* title = "Title";

    std::vector<int> data(100);
    std::iota(data.begin(), data.end(), 1);

    float chartAreaRatio = 0.80f;
    int chartWidth = static_cast<int>(windowWidth * chartAreaRatio);
    int chartHeight = static_cast<int>(windowHeight * chartAreaRatio);
    int chartX = (windowWidth - chartWidth) / 2;
    int chartY = (windowHeight - chartHeight) / 2;
    SDL_Rect rect {chartX, chartY, chartWidth, chartHeight};

    Chart chart(rect, data.cbegin(), data.cend());

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0x00,   0x3f,   0x5c, 255);

    
	SDL_RenderClear(renderer);

    chart.draw(renderer);

	SDL_RenderPresent(renderer);

    SDL_Delay(10000);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();


    return 0;
}
