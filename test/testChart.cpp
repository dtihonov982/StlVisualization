#include "Chart.h"
#include <numeric>
#include <SDL2/SDL.h>
#include <algorithm>

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
    for (int i = 0; i < 100; ++i) {
        SDL_Delay(50);
        std::rotate(data.begin(), data.end() - 1, data.end());
        chart.update(data.begin(), data.end());

        SDL_SetRenderDrawColor(renderer, 0x00,   0x3f,   0x5c, 255);
        SDL_RenderClear(renderer);

        chart.draw(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();


    return 0;
}
