#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <iostream>

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
    //this opens a font style and sets a size
    TTF_Font* Sans = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 25);
    if (!Sans) {
        std::cout << TTF_GetError() << "\n";
        return 1;
    }
    //check null

    // this is the color in rgb format,
    // maxing out all would give you the color white,
    // and it will be your text's color
    SDL_Color White = {255, 255, 255};

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid_Wrapped(Sans, "put your text here", White, 0); 
    //check null

    // now you can convert it into a texture
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    //check null SDL_GetError

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 10;  //controls the rect's x coordinate 
    Message_rect.y = 10; // controls the rect's y coordinte
    SDL_QueryTexture(Message, nullptr, nullptr, &Message_rect.w, &Message_rect.h);

    // (0,0) is on the top left of the window/screen,
    // think a rect as the text's box,
    // that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy
    // in your game loop area, the area where the whole code executes

    // you put the renderer's name first, the Message,
    // the crop size (you can ignore this if you don't want
    // to dabble with cropping), and the rect which is the size
    // and coordinate of your texture
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);


	SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    //
    // Don't forget to free your surface and texture
    TTF_CloseFont(Sans);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

    return 0;

}
