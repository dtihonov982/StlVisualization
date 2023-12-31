#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "Visual/Label.h"

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


    TTF_Init();

    Label label(renderer);
    label.setText("Some text here.");
    label.setFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 32);
    label.setColor({0xff, 0xff, 0xff, 0xff});
    label.update();
    int labelWidth = label.getWidth();
    int labelHeight = label.getHeight();
    label.setPos(width / 2 - labelWidth / 2, height / 2 - labelHeight / 2);
    SDL_Rect labelRect = label.getRect();
    SDL_SetRenderDrawColor( renderer, 0x88, 0x88, 0x88, 0x88);
    SDL_RenderFillRect(renderer, &labelRect);
    label.draw();
    

	SDL_RenderPresent(renderer);
    SDL_Delay(3000);

	SDL_DestroyWindow(window);
    std::cerr << SDL_GetError() << "\n";
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

    return 0;

}
