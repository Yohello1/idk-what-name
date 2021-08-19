#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define WINDOW_WIDTH 600

int noise(void);
struct positions
{
    int y_pos;
    int x_pos;
    int r;
    int g;
    int b;
};
int noise_run = 50;
int currenttime = (unsigned int)time(NULL);

int main(void)
{
    std::cout << "Time = " << currenttime << "\n";
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    int amount_of_points = 100;
    struct positions spots[amount_of_points];

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int positions_to_generate = 0; positions_to_generate < amount_of_points; positions_to_generate++)
    {
        spots[positions_to_generate].x_pos = noise() % WINDOW_WIDTH;
        spots[positions_to_generate].y_pos = noise() % WINDOW_WIDTH;
        spots[positions_to_generate].r = noise() % 255;
        spots[positions_to_generate].g = noise() % 255;
        spots[positions_to_generate].b = noise() % 255;
        std::cout << "Should be generating " << positions_to_generate << "\n";
    }
    std::cout << "Should be done generating\n";
    for (int i = 0; i < amount_of_points; ++i)
    {
        SDL_SetRenderDrawColor(renderer, spots[i].r, spots[i].g, spots[i].b, 255);
        SDL_RenderDrawPoint(renderer, spots[i].x_pos, spots[i].x_pos);
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done\n";
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

int noise()
{
    // srand(currenttime + noise_run);
    const int seed = currenttime;
    std::mt19937_64 mtrand(seed);
    int return_int = mtrand();
    return return_int;
}