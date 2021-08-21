#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define LOGICAL_WINDOW_WIDTH 512
#define ACTUAL_WINDOW_WIDTH 1024

// Struct
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
    positions *new_state = new positions[amount_of_points];
    std::cout << "We have now declared the arrays and stuff\n";
    srand(currenttime);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // WHY IS THERE SO MUCH STUFF JUST TO INIT THIS

    int position = 0;

    // Putting down the noise
    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            position++;

            // This should be self explainatory???
            // gets position
            spots[position].x_pos = x;
            spots[position].y_pos = y;
            // Sets `colour` var to either 0 or 255 then it gets assigned to r,g, and b
            int colour = noise_gen() * 255;
            spots[position].r = colour;
            spots[position].g = colour;
            spots[position].b = colour;
            // Prints this so I can see what is going on
            std::cout << "Should be generating " << position << "\n"
                      << "X = " << spots[position].x_pos << "\n"
                      << "Y = " << spots[position].y_pos << "\n";
            SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
            std::cout << "Point (" << x << " , " << y << ") \n";
        }
    }

    // Well, present it
    SDL_RenderPresent(renderer);
    std::cout << "Done Noise? phase\n";
    for (int itterations = 0; itterations < 10; itterations++)
    {
        position = 0;
        int walled = 0;

        for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
        {
            for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
            {
                position++;

                walled = 0;

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

                    if (spots[position - LOGICAL_WINDOW_WIDTH + 1].r == 255)
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
                if (walled > 4)
                {
                    std::cout << " Becomes wall\n";
                    new_state[position].r = 255;
                    new_state[position].g = 255;
                    new_state[position].b = 255;
                    new_state[position].x_pos = spots[position].x_pos;
                    new_state[position].y_pos = spots[position].y_pos;
                }
                else
                {
                    std::cout << "\n";
                    new_state[position].r = 0;
                    new_state[position].g = 0;
                    new_state[position].b = 0;
                    new_state[position].x_pos = spots[position].x_pos;
                    new_state[position].y_pos = spots[position].y_pos;
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
                // THIS IS WHERE IT WILL GET MESSED UP
                // But WHY
                position++;

                // new_state[position].r = spots[position].r;
                // new_state[position].g = spots[position].g;
                // new_state[position].b = spots[position].b;

                spots[position].r = new_state[position].r;
                spots[position].g = new_state[position].g;
                spots[position].b = new_state[position].b;
                std::cout << " NEXT ITTERATION\nSpots ( " << spots[position].x_pos << "," << spots[position].y_pos << ")\n"
                          << "New_state ( " << new_state[position].x_pos << "," << new_state[position].y_pos << ")\n";

                SDL_SetRenderDrawColor(renderer, spots[position].r, spots[position].g, spots[position].b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        SDL_RenderPresent(renderer);

        std::cout << "Done hood phase\n";
    }

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
    delete[] new_state;

    return EXIT_SUCCESS;
}

int noise_gen()
{
    srand(currenttime + time_noise_run);
    time_noise_run++;
    int num = (rand() % 100);

    if (num > 40)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}