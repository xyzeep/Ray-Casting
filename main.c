#include <SDL3/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    // if something unexpected happens when initializing
    if (result < 0)
    {
        SDL_Log("SDL_Init Error %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Ray Tracing", 800, 600, 0);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow Error %s", SDL_GetError());
        return -2;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer Error %s", SDL_GetError());
        return -3;
    }

    SDL_Log("SDL3 initialized.");

    SDL_Event event;
    int quit = 0; // assigning a variable to quiting

    // main loop
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                SDL_Log("SDL3 Event Quit");
                quit = 1;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    SDL_Log("SDL3 ESC key Quit");
                    quit = 1;
                }
            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff); // black color
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1); // prevents CPU from being fully consumed by the game loop
    }

    // cleaning up when window is closed
    SDL_Log("SDL3 shutdown");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}