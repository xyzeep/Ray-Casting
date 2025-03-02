#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define NUMBER_OF_RAYS 100
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_RAY 0xffd4d4d4


struct Circle
{
    double x, y;
    double r;
    int direction; // 0 is up 1 is down
    bool canMove;
};

struct Ray
{
    double startX, startY;
    double endX, endY;
    double angle;
};

// function to draw a desired circle
void drawCircle(struct Circle circle, SDL_Surface *surface)
{
    double radius_squared = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {

        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            // if it satisfies the eqn of a circle
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared <= radius_squared)
            {
                SDL_Rect point = {x, y, 1, 1};
                SDL_FillSurfaceRect(surface, &point, COLOR_WHITE);
            }
        }
    }
}

// function to move the light source
void moveCircle(struct Circle *circle)
{
    float cursor_x, cursor_y;
    SDL_GetMouseState(&cursor_x, &cursor_y);

    double radius_squared = pow(circle->r, 2);
    double cursor_distance_squared = pow(cursor_x - circle->x, 2) + pow(cursor_y - circle->y, 2);
    // if the cursor is inside the circle
    if (!circle->canMove)
    {
        if (cursor_distance_squared <= radius_squared)
        {
            circle->canMove = true;
            if (cursor_x >= 0 && cursor_x <= SCREEN_WIDTH)
            {
                circle->x = cursor_x;
            }
            if (cursor_y >= 0 && cursor_y <= SCREEN_HEIGHT)
            {
                circle->y = cursor_y;
            }
        }
    }
    else
    {

        if (cursor_x >= 0 && cursor_x <= SCREEN_WIDTH)
        {
            circle->x = cursor_x;
        }
        if (cursor_y >= 0 && cursor_y <= SCREEN_HEIGHT)
        {
            circle->y = cursor_y;
        }
    }
}

// function to animate the opaqueCircle
void animateOpaqueCircle(struct Circle *circle)
{
    double aniSpeed = 12;

    if (circle->direction == 0)
    {
        circle->y -= aniSpeed;
    }
    else if (circle->direction == 1)
    {
        circle->y += aniSpeed;
    }

    if (circle->y <= circle->r)
    {
        circle->direction = 1;
    }
    else if (circle->y >= SCREEN_HEIGHT - circle->r)
    {
        circle->direction = 0;
    }
}

// function to draw rays
void drawRectRays(SDL_Surface *surface, struct Ray rays[], struct Circle opaqueCirc)
{
    double opaqueRadiusSquared = pow(opaqueCirc.r, 2);
    int thickness = 2;

    // skip steps in rays for optimized rendering
    int step = 8;
    for (int i = 0; i < NUMBER_OF_RAYS; i++)
    {
        double ray_length = sqrt(pow((rays[i].endX - rays[i].startX), 2) + pow((rays[i].endY - rays[i].startY), 2));

        // direction of the ray (how much is x/y changing in each step)
        double dx = (rays[i].endX - rays[i].startX) / ray_length;
        double dy = (rays[i].endY - rays[i].startY) / ray_length;

        bool drawable = true;
        // each segment (point) of current ray
        for (double j = 0; j < ray_length; j += step)
        {
            int x = (int)(rays[i].startX + dx * j);
            int y = (int)(rays[i].startY + dy * j);
            // check if it's overlapping with the opaque circle
            double distance_squared = pow(x - opaqueCirc.x, 2) + pow(y - opaqueCirc.y, 2);
            if (distance_squared <= opaqueRadiusSquared)
            {
                break;
            }
            else
            {
                // draw ray
                SDL_Rect rect = {x, y, thickness, thickness};
                SDL_FillSurfaceRect(surface, &rect, COLOR_RAY);
            }
        }
    }
}
// function to generate and store rays data
void generateRays(struct Circle circle, struct Ray rays[])
{
    double angle_spacing = 360.0 / NUMBER_OF_RAYS;
    double lineX;
    double lineY;
    double line_angle;
    for (int i = 0; i < NUMBER_OF_RAYS; i++)
    {
        line_angle = angle_spacing * i;

        lineX = ((circle.r + SCREEN_WIDTH * 2) * cos(line_angle * M_PI / 180)) + circle.x;
        lineY = ((circle.r + SCREEN_HEIGHT * 2) * sin(line_angle * M_PI / 180)) + circle.y;

        // store ray data in array
        rays[i].startX = circle.x;
        rays[i].startY = circle.y;
        rays[i].endX = lineX;
        rays[i].endY = lineY;
        rays[i].angle = line_angle;
    }
}

int main()
{
    // initializing SDL
    SDL_Init(SDL_INIT_VIDEO); // we will only need video feature for this program
                              // creating window and surface
    SDL_Window *window = SDL_CreateWindow("Rays", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    // light source circle
    struct Circle lightCircle = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 60, false};
    // lightRays
    struct Ray lightRays[NUMBER_OF_RAYS]; // array to store the rays
                                          // opaque object
    struct Circle opaqueCircle = {1000, SCREEN_HEIGHT / 2, 100, 0};

    int quit = 0;
    SDL_Event event;

    int mousePressed = 0;
    int mouseMoving = 0;

    // main loop
    while (!quit)
    {
        // re-draw the surface black
        SDL_Rect black_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_FillSurfaceRect(surface, &black_rect, COLOR_BLACK);

        // generate rays
        generateRays(lightCircle, lightRays);

        // draw rays coming from it
        drawRectRays(surface, lightRays, opaqueCircle);

        // drawing lightCircle
        drawCircle(lightCircle, surface);

        // draw opaque circle
        drawCircle(opaqueCircle, surface);

        // animate the opaquecircle
        animateOpaqueCircle(&opaqueCircle);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
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
            // if the mouse button is pressed and moving
            if (mousePressed)
            {
                moveCircle(&lightCircle);
            }
        }


        //updating the window surface
        SDL_UpdateWindowSurface(window);

        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

