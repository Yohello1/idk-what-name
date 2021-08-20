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
    struct positions new_state[amount_of_points];
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
            SDL_RenderDrawPoint(renderer, x, y);
            std::cout << "Point (" << x << " , " << y << ") \n";
        }
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done Noise? phase\n";

    position = 0;
    int walled = 0;

    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            walled = 0;
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
            // 2 1 3
            // 4 X 5
            // 7 6 8

            // Top
            if (spots[position].y_pos != 0)
            {
                if (spots[position - LOGICAL_WINDOW_WIDTH].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }
            if (spots[position].y_pos != 0 && spots[position].x_pos != 0)
            {

                if (spots[position - LOGICAL_WINDOW_WIDTH - 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }
            if (spots[position].y_pos != 0 && spots[position].x_pos != 255)
            {

                if (spots[position - LOGICAL_WINDOW_WIDTH + 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }

            //Mid
            if (spots[position].x_pos != 0)
            {
                if (spots[position - 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }
            if (spots[position].x_pos != 255)
            {
                if (spots[position + 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }

            // Bottom
            if (spots[position].y_pos != 255)
            {
                if (spots[position + LOGICAL_WINDOW_WIDTH].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }
            if (spots[position].y_pos != 255 && spots[position].x_pos != 0)
            {

                if (spots[position - LOGICAL_WINDOW_WIDTH - 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }
            if (spots[position].y_pos != 255 && spots[position].x_pos != 255)
            {

                if (spots[position - LOGICAL_WINDOW_WIDTH - 1].r == 255)
                {
                    walled++;
                }
            }
            else
            {
                walled++;
            }

            std::cout << "Position ( " << x << "," << y << " )"
                      << "Walls = " << walled;
            if (walled > 5)
            {
                std::cout << " Becomes wall\n";
                new_state[position].r = 255;
                new_state[position].g = 255;
                new_state[position].b = 255;
            }
            else
            {
                std::cout << "\n";
                new_state[position].r = 0;
                new_state[position].g = 0;
                new_state[position].b = 0;
            }
        }
    }

    std::cout << "Done wall phase\n";
    SDL_Delay(5000);

    position = 0;
    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;
            spots[position].r = new_state[position].r;
            spots[position].g = new_state[position].g;
            spots[position].b = new_state[position].b;
            SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);

    std::cout << "Done hood phase\n";


    

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

    if (num > 50)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}