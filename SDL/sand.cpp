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
    std::cout << "Done inizaling everything\n";

    // WHY IS THERE SO MUCH STUFF JUST TO INIT THIS
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

    for (int itterations = 0; itterations < 1000; itterations++)
    {
        std::cout << "Itterations = " << itterations;
        position = 0;

        for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
        {
            for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
            {
                position++;

                // There should be another struct for every position
                // Therefore, if we get the position I am currently at, and subtract
                // the width of it from the index of the struct array we should get the part above it
                // and add it to the thingy to get the one below it, then +/- 1 to get the other ones
                // if x = 0 skip checking the ones on the left
                // if x = 255 skip the right
                // if y = 0 skip top check
                // if y = 255 skip bottom check
                // this is gonna have so many branches
                // X X X
                // X X X
                // X 1 X
                // Bottom
                if (spots[position].y_pos != 255)
                {
                    if (spots[position + LOGICAL_WINDOW_WIDTH].empty == false)
                    {
                        spots[position + LOGICAL_WINDOW_WIDTH].empty = true;
                        spots[position + LOGICAL_WINDOW_WIDTH].r = 255;
                        spots[position + LOGICAL_WINDOW_WIDTH].g = 255;

                        spots[position].empty = true;
                        spots[position].r = 0;
                        spots[position].g = 0;
                    }
                }

                // std::cout << "Position ( " << x << "," << y << " )"
                //           << "Walls = " << walled;
                // if (walled > 4)
                // {
                //     std::cout << " Becomes wall\n";
                //     new_state[position].r = 255;
                //     new_state[position].g = 255;
                //     new_state[position].b = 255;
                //     new_state[position].x_pos = spots[position].x_pos;
                //     new_state[position].y_pos = spots[position].y_pos;
                // }
                // else
                // {
                //     std::cout << "\n";
                //     new_state[position].r = 0;
                //     new_state[position].g = 0;
                //     new_state[position].b = 0;
                //     new_state[position].x_pos = spots[position].x_pos;
                //     new_state[position].y_pos = spots[position].y_pos;
                // }
                //
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
        SDL_Delay(100);
        SDL_RenderPresent(renderer);
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
    delete[] spots;

    return EXIT_SUCCESS;
}