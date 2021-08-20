#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

struct positions
{
    int y_pos;
    int x_pos;
    int r;
    int g;
    int b;
    int a;
};
int noise_gen();
unsigned int currenttime = (unsigned int)time(NULL);
unsigned int time_noise_run = 0;

int main(void)
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    std::cout << "Time = " << currenttime << "\n";
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    int amount_of_points = LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH;
    struct positions spots[amount_of_points];
    struct positions new[amount_of_points];
    srand(currenttime);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // for(int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)

    int position = 0;

    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;

            spots[position].x_pos = x;
            spots[position].y_pos = y;
            int colour = noise_gen() * 255;
            spots[position].r = colour;
            spots[position].g = colour;
            spots[position].b = colour;
            std::cout << "Should be generating " << position << "\n"
                      << "X = " << spots[position].x_pos << "\n"
                      << "Y = " << spots[position].y_pos << "\n";
            SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
             SDL_RenderPresent(renderer);

            SDL_RenderDrawPoint(renderer, x, y);
            std::cout << "Point (" << x << " , " << y << ") \n";
        }
    }
    SDL_RenderPresent(renderer);








    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

int noise_gen()
{
    srand(currenttime + time_noise_run);
    time_noise_run++;
    int num = (rand() % 100);

    if (num > 90)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}