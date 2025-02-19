#include <SDL3/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main()
{
   
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    // initializing SDL
    SDL_Init(SDL_INIT_VIDEO);
    //creating window and renderer
    SDL_CreateWindowAndRenderer("My Window",800,500, 0, &window, &renderer);
    // black color to draw the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer); //clear screen to black

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // setting the draw color to white
   
    // drawing a rectangle
    SDL_FRect whiteRect= {800/4, 500/2, 100, 100};
    SDL_RenderFillRect(renderer, &whiteRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red color
   // drawing a rectangle
    SDL_FRect redRect= {800/2, 500/2, 100, 100};
    SDL_RenderFillRect(renderer, &redRect);


    SDL_RenderPresent(renderer); 


    SDL_Delay(5000); // delay of 10secs before window closes

    return 0;
       
}


