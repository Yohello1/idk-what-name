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

void render_now(void);
void excecution_finished(void);
void fluid_1();
void scr_dump();
void draw_and_render();
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

    // Makt a rectangle of water??
    for (int x_pos = 40; x_pos < 220; x_pos++)
    {
        for (int y_pos = 40; y_pos < 120; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            pixels[x_pos][y_pos].state_now = fluid;
            pixels[x_pos][y_pos].r = 0;
            pixels[x_pos][y_pos].g = 0;
            pixels[x_pos][y_pos].b = 255;
        }
    }
    std::cout << "Does this work?" << std ::endl;
    if (pixels[500][300].state_now == empty)
    {
        std::cout << "this should be false" << std::endl;
        SDL_RenderDrawPoint(renderer, 500, 300);
        pixels[500][300].state_now = fluid;
    }
    std::cout << "Yes it does?" << std ::endl;

    // // Make a rectangle of sand
    // for (int x_pos = 80; x_pos < 100; x_pos++)
    // {
    //     for (int y_pos = 120; y_pos < 250; y_pos++)
    //     {
    //         SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //         SDL_RenderDrawPoint(renderer, x_pos, y_pos);

    //         pixels[x_pos][y_pos].state_now = solid;
    //         pixels[x_pos][y_pos].r = 255;
    //         pixels[x_pos][y_pos].g = 255;
    //         pixels[x_pos][y_pos].b = 0;
    //     }
    // }

    SDL_RenderPresent(renderer);
    std::cout << "Done1\n";
    SDL_Delay(1000);
    // Note: This is where the actual phsyics part happens for sand
    for (int iterations = 0; iterations < 1000; iterations++)
    {
        fluid_1();
        SDL_Delay(20);
        draw_and_render();
        std::cout << "Iteration : " << iterations << std::endl;
    }
    // scr_dump();
    std::cout << "Done\n";
    // Wait till ordered to kill the process, and free everything from the shackles of this computer world
    excecution_finished();
    return EXIT_SUCCESS;
}
void render_now()
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
        // SDL_Delay(50);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void fluid_1(void)
{
    std::copy(&pixels[0][0], &pixels[0][0] + LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH, &new_version[0][0]);

    for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {

            // Check below and move.
            if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty && y_pos != (LOGICAL_WINDOW_WIDTH - 1))
            {
                new_version[x_pos][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos][y_pos + 1].b = 255;

                new_version[x_pos][y_pos].state_now = empty;
                new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            }
            // Down left
            else if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty && y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1)
            {
                new_version[x_pos - 1][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos - 1][y_pos + 1].b = 255;

                new_version[x_pos][y_pos].state_now = empty;
                new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            }
            // Down right
            else if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos + 1][y_pos + 1].state_now == empty && new_version[x_pos + 1][y_pos + 1].state_now == empty && y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 250)
            {
                new_version[x_pos + 1][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos + 1][y_pos + 1].b = 255;

                new_version[x_pos][y_pos].state_now = empty;
                new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            }
            // Should be right?
            // else if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos + 1][y_pos].state_now == empty && new_version[x_pos + 1][y_pos].state_now == empty && x_pos != 250)
            // {
            //     new_version[x_pos + 1][y_pos] = pixels[x_pos][y_pos];
            //     new_version[x_pos + 1][y_pos].b = 255;

            //     new_version[x_pos][y_pos].state_now = empty;
            //     new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            // }

            else if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos + 1][y_pos].state_now == empty && x_pos != 250 && new_version[x_pos + 1][y_pos].state_now == empty)
            {
                new_version[x_pos + 1][y_pos] = pixels[x_pos][y_pos];
                new_version[x_pos + 1][y_pos].b = 255;

                new_version[x_pos][y_pos].state_now = empty;
                new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            }

            else if (pixels[x_pos][y_pos].state_now == fluid && pixels[x_pos - 1][y_pos].state_now == empty && x_pos != 250 && new_version[x_pos - 1][y_pos].state_now == empty)
            {
                new_version[x_pos - 1][y_pos] = pixels[x_pos][y_pos];
                new_version[x_pos - 1][y_pos].b = 255;

                new_version[x_pos][y_pos].state_now = empty;
                new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].r = 0;
            }
        }
    }
    // scr_dump();
    std::copy(&new_version[0][0], &new_version[0][0] + LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH, &pixels[0][0]);
    SDL_RenderPresent(renderer);
}

void scr_dump()
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            std::cout << pixels[x_pos][y_pos].state_now;
        }
        std::cout << std::endl;
    }
}

void draw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            if (pixels[x_pos][y_pos].b == 255)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }
}