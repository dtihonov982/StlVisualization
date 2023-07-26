#include <SDL.h>

class App {
public:
    App(const Config& conf, const std::vector<int>& data);
    ~App();
    void update();
    void render();
private:
    SDL_Window* window_;
    std::vector<Rectangle> rectangles_;
    SDL_Renderer* renderer_;
};
