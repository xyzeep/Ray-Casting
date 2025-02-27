#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 1400 
#define SCREEN_HEIGHT 800
#define NUMBER_OF_RAYS 50

struct Circle {
    double x, y;
    double r;
    int direction; // 0 is up 1 is down
    bool canMove;
};

struct Ray{
    double startX, startY;
    double endX, endY;
    double angle;
};    

// a beautiful function to draw a desired circle :)
void drawCircle(struct Circle circle,SDL_Renderer* renderer) {


    SDL_SetRenderDrawColor(renderer, 193, 219, 218, 164);
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {

        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y ++) {
            // if it satisfies the eqn of a circle
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if(distance_squared <= radius_squared) {
                SDL_RenderPoint(renderer, x, y);
            }
        }
    }
}
//function to move the light source
void moveCircle(struct Circle* circle) {
    float cursor_x, cursor_y;
    SDL_GetMouseState(&cursor_x, &cursor_y);

    double radius_squared = pow(circle->r, 2);
    double cursor_distance_squared = pow(cursor_x - circle->x, 2) + pow(cursor_y - circle->y, 2);
    //if the cursor is inside the circle
    if(!circle->canMove){
        if(cursor_distance_squared <= radius_squared) {
            circle->canMove = true;
            if (cursor_x >= 0 && cursor_x <= SCREEN_WIDTH) {
                circle->x = cursor_x;
            }    
            if (cursor_y >=0 && cursor_y <= SCREEN_HEIGHT) {
                circle->y = cursor_y;
            }    
        }
    }    
    else {

        if (cursor_x >= 0 && cursor_x <= SCREEN_WIDTH) {
            circle->x = cursor_x;
        }    
        if (cursor_y >=0 && cursor_y <= SCREEN_HEIGHT) {
            circle->y = cursor_y;
        }   
    }   

}  

void animateOpaqueCircle(struct Circle* circle) {

    if (circle->direction == 0) {
        circle->y -= 1;    
    }    
    else if (circle->direction == 1){
        circle->y += 1;
    }    

    if (circle->y <= circle->r){
        circle->direction = 1;
    }
    else if (circle->y >= SCREEN_HEIGHT - circle->r){
        circle->direction = 0;
    }    


}    


//fucntion to draw rays using rects (for custom thickness)
//void drawRectRays(SDL_Renderer* renderer, struct Ray rays[]) {
//    SDL_SetRenderDrawColor(renderer, 252, 252, 141, 255);
//   for(int i = 0; i < NUMBER_OF_RAYS; i++) {
//        double ray_length = sqrt(pow((rays[i].endX - rays[i].startX), 2)+pow((rays[i].endY - rays[i].startY), 2));
//        for (double j = 0; j < ray_length; j++)
//       }    
//    }    


//draw rays functions(using lines)
void drawRays(SDL_Renderer* renderer, struct Ray rays[]) {
    SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
    for(int i = 0; i < NUMBER_OF_RAYS; i++) {
        SDL_RenderLine(renderer, rays[i].startX, rays[i].startY, rays[i].endX, rays[i].endY); 

    }    
}    


void generateRays(struct Circle circle, struct Ray rays[]){
    double angle_spacing = 360.0 / NUMBER_OF_RAYS;
    double lineX;
    double lineY;
    double line_angle;
    for(int i = 0; i < NUMBER_OF_RAYS; i++){
        line_angle =  angle_spacing * i;

        lineX = ((circle.r + SCREEN_WIDTH * 2) * cos(line_angle * M_PI/180)) + circle.x;
        lineY = ((circle.r + SCREEN_HEIGHT* 2) * sin(line_angle * M_PI/180)) + circle.y;

        //store ray data in array
        rays[i].startX = circle.x;
        rays[i].startY = circle.y;
        rays[i].endX = lineX;
        rays[i].endY = lineY;
        rays[i].angle = line_angle;
    }   
}

int main()
{

    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    // initializing SDL
    SDL_Init(SDL_INIT_VIDEO);
    //creating window and renderer
    SDL_CreateWindowAndRenderer("RAY TRACING",SCREEN_WIDTH,SCREEN_HEIGHT, 0, &window, &renderer);

    // light source circle
    struct Circle lightCircle = {SCREEN_WIDTH/4, SCREEN_HEIGHT/2, 60, false};
    struct Ray lightRays[NUMBER_OF_RAYS]; // array to store the rays

    //opaque object
    struct Circle opaqueCircle = {1000, SCREEN_HEIGHT/2, 100, 0};

    int quit = 0;
    SDL_Event event;

    int mousePressed = 0;
    int mouseMoving = 0;
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
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mousePressed = 1;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mousePressed = 0;
                    lightCircle.canMove = false;
                    break;
                default:
                    break;
            }
            //if the mouse button is pressed and moving
            if (mousePressed) 
            {
                moveCircle(&lightCircle);
            }    

        }
        //generate rays
        generateRays(lightCircle, lightRays);
        //draw rays coming from it
        drawRays(renderer, lightRays);
        // drawing lightCircle
        drawCircle(lightCircle, renderer);
        //draw opaque circle
        drawCircle(opaqueCircle, renderer);
        //animate the opaquecircle
        animateOpaqueCircle(&opaqueCircle);

        // commit the renders or that kinda thing
        SDL_RenderPresent(renderer); 

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}



