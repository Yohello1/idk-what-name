#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define LOGICAL_WINDOW_WIDTH 256

//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
enum pixel_state
{
    empty,
    gas,
    fluid,
    solid,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    int r;
    int g;
    int b;
    int a;
};
// Array of pixel stuff

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];

void draw_and_render(void);
void excecution_finished(void);
// x ranges, y ranges, and rgba colour
// s = smaller, x = larger

int main()
{
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // *rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1024, 1024, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    // PAINT IT BLACK
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // int total_droplets = 100;
    // int droplets_remaining = total_droplets;
    // for (int x = 0; x < LOGICAL_WINDOW_WIDTH ; x++)
    // {
    //     //blue
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    //     SDL_RenderDrawPoint(renderer, LOGICAL_WINDOW_WIDTH / 2, x);

    //     //black
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //     SDL_RenderDrawPoint(renderer, LOGICAL_WINDOW_WIDTH / 2, x - 1 - total_droplets);

    //     // present
    //     SDL_RenderPresent(renderer);
    //     SDL_Delay(5);
    //     droplets_remaining--;
    // }

    // Wait till ordered to kill the process, and free everything from the shackles of this computer world

    for (int x_pos = 120; x_pos < 125; x_pos++)
    {
        for (int y_pos = 40; y_pos < 100; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            pixels[x_pos][y_pos].state_now = solid;
            pixels[x_pos][y_pos].r = 0;
            pixels[x_pos][y_pos].g = 0;
            pixels[x_pos][y_pos].b = 255;
        }
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done1\n";
    // Note: This is where the actual phsyics part happens for sand
    for (int iterations = 0; iterations < 5000; iterations++)
    {
        for (int x_pos = LOGICAL_WINDOW_WIDTH; x_pos > 0; x_pos--)
        {
            for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
            {
                if (pixels[x_pos][y_pos].state_now == solid && y_pos != (LOGICAL_WINDOW_WIDTH - 1))
                {
                    // Now the thingy

                    //Down
                    if (pixels[x_pos][y_pos + 1].state_now == empty && y_pos != (LOGICAL_WINDOW_WIDTH -1) && new_version[x_pos][y_pos + 1].state_now == empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos + 1);
                        new_version[x_pos][y_pos + 1].state_now = solid;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        new_version[x_pos][y_pos].state_now = empty;
                    }
                    // Left Down
                    else if (pixels[x_pos - 1][y_pos + 1].state_now == empty && x_pos != 1 && new_version[x_pos - 1][y_pos + 1].state_now == empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderDrawPoint(renderer, x_pos - 1, y_pos + 1);
                        new_version[x_pos - 1][y_pos + 1].state_now = solid;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        new_version[x_pos][y_pos].state_now = empty;
                    }
                    // Right down
                    else if (pixels[x_pos + 1][y_pos + 1].state_now == empty && x_pos != (LOGICAL_WINDOW_WIDTH - 1) && new_version[x_pos + 1][y_pos + 1].state_now == empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderDrawPoint(renderer, x_pos + 1, y_pos + 1);
                        new_version[x_pos + 1][y_pos + 1].state_now = solid;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        new_version[x_pos][y_pos].state_now = empty;
                    }
                    // Left
                    else if (pixels[x_pos + 1][y_pos].state_now == empty && x_pos != (LOGICAL_WINDOW_WIDTH - 1) && new_version[x_pos + 1][y_pos].state_now == empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderDrawPoint(renderer, x_pos + 1, y_pos);
                        new_version[x_pos + 1][y_pos].state_now = solid;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        new_version[x_pos][y_pos].state_now = empty;
                    }
                    // Right
                    else if (pixels[x_pos - 1][y_pos].state_now == empty && x_pos != 1&& new_version[x_pos - 1][y_pos].state_now == empty)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderDrawPoint(renderer, x_pos - 1, y_pos);
                        new_version[x_pos - 1][y_pos].state_now = solid;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        new_version[x_pos][y_pos].state_now = empty;
                    }
                    else
                    {
                        new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
                    }
                }
            }
        }

        std::copy(&new_version[0][0], &new_version[0][0] + LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH, &pixels[0][0]);
        // new_version = pixels;
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        std::cout << iterations << std::endl;
    }

    std::cout << "Done\n";
    excecution_finished();
    return EXIT_SUCCESS;
}
void draw_and_render()
{
    SDL_RenderPresent(renderer);
}
void excecution_finished(void)
{
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
