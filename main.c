#include <SDL3/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main() {
    //creating a window
    SDL_Window *window = SDL_CreateWindow("RayTracing", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // screen loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }
    }

    // cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
