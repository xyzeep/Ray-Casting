#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 1400 
#define SCREEN_HEIGHT 800

struct Circle {
    double x, y;
    double r;
};

// a beautiful function to draw a desired circle :)
void drawCircle(struct Circle circle,SDL_Renderer* renderer) {
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {

        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y ++) {
            // if it satisfies the eqn of a circle
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if(distance_squared <= radius_squared) {

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderPoint(renderer, x, y);
            }
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        }
    }
}

// a beautiful function to draw lines passing thru the center of a circle
void drawRays(struct Circle circle,SDL_Renderer* renderer, double spacing) {
    for (double x = 0; x <= SCREEN_WIDTH; x+= spacing) {

        for (double y = 0; y <= SCREEN_HEIGHT; y += spacing) {
            SDL_RenderLine(renderer, x, y, circle.x, circle.y);
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    }
}
int main()
{

    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    // initializing SDL
    SDL_Init(SDL_INIT_VIDEO);
    //creating window and renderer
    SDL_CreateWindowAndRenderer("My Window",SCREEN_WIDTH,SCREEN_HEIGHT, 0, &window, &renderer);

    // light source circle
    struct Circle lightCircle = {SCREEN_WIDTH/4, SCREEN_HEIGHT/2, 80};

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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // drawing lightCircle
        drawCircle(lightCircle, renderer);
        //draw rays coming from it
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawRays(lightCircle, renderer, 10);
        // commit the renders or that kinda thing
        SDL_RenderPresent(renderer); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}



