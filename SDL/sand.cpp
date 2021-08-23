#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

unsigned int currenttime = (unsigned int)time(NULL);

struct positions
{
    int y_pos;
    int x_pos;
    bool empty;
    int r;
    int g;
    int b;
    int a;
};
int main()
{
    // A lot of init stuff
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    std::cout << "Time = " << currenttime << "\n";
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::cout << "Declared Event, Renderer, and Window pointers but un assigned\n";
    int amount_of_points = LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH;
    // struct positions spots[];
    // struct positions new_state[amount_of_points];
    positions *spots = new positions[amount_of_points];
    std::cout << "We have now declared the arrays and stuff\n";
    srand(currenttime);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                
    // spots[position].r = spots[position - 15].r;
                // spots[position].g = spots[position - 15].g;
                // spots[position].b = spots[position - 15].b;
                // spots[position].a = spots[position - 15].a;
    int position = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;
            spots[position].x_pos = x;
            spots[position].y_pos = y;
            spots[position].empty = true;
            spots[position].r = 0;
            spots[position].g = 0;
            spots[position].b = 0;
            spots[position].a = 255;
            // SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done setting base vars\n";

    position = 0;
    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;
            if (124 < spots[position].x_pos && spots[position].x_pos < 132)
            {
                if (20 < spots[position].y_pos && spots[position].y_pos < 40)
                {
                    spots[position].empty = false;
                    spots[position].r = 255;
                    spots[position].g = 255;
                    std::cout << "Point which will be yellow (" << x << "," << y << ")\n";
                }
            }
        }
    }
    std::cout << "Done the weird if statement\n";

    position = 0;
    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;
            SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done the writing\n";
    SDL_Delay(10000);

    position = 0;
    // for (int iteration = 0; iteration < 100; iteration++)
    // {
        // std::cout << "iteration = " << iteration << std::endl;
        position = 0;

        for (int y = LOGICAL_WINDOW_WIDTH; y > 0; y--)
        {

            for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
            {

                position++;
                // spots[position].r = spots[position - 15].r;
                // spots[position].g = spots[position - 15].g;
                // spots[position].b = spots[position - 15].b;
                // spots[position].a = spots[position - 15].a;

                SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
                SDL_RenderDrawPoint(renderer, x, y);

                // spots[position].r = 0;
                // spots[position].g = 0;
                // spots[position].b = 0;
                // spots[position].a = 255;
            }
        }

        position = 0;

        for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
        {
            for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
            {
                position++;
                SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        SDL_RenderPresent(renderer);
    // }
        std::cout << "Done the writing2\n";

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
    delete[] spots;

    return EXIT_SUCCESS;
}