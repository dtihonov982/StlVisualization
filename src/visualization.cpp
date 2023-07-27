#include <SDL.h>


int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width = 400;
    int height = 320;
    const char* title = "Title";
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, 
                    SDL_WINDOWPOS_CENTERED, 
                    SDL_WINDOWPOS_CENTERED, 
                    width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    
	SDL_RenderClear(renderer);

    SDL_Rect rect = {width / 4, height / 4, width / 2, height / 2 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);

    SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

    return 0;
}
