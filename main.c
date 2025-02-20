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
   
   int quit = 0;
   SDL_Event event;
   //main loop
    while (!quit) {
        // black color to draw the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
        SDL_RenderClear(renderer); //clear screen to black
        
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    SDL_Log("SDL Quit.");
                    quit = 1; 
            }
        }
        

    SDL_RenderPresent(renderer); 
    }

    SDL_Quit();

    return 0;

}


