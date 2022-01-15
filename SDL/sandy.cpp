#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define LOGICAL_WINDOW_WIDTH 256

//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

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
// Array of pixel stuff

positions *pixels = new positions[(LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH)];

void draw_and_render(void);
void excecution_finished(void);
// x ranges, y ranges, and rgba colour
// s = smaller, x = larger
void draw_box(int x_range_s, int x_range_x, int y_range_s, int y_range_x, int r_val, int g_val, int b_val, int a_val);
void sand_sim(void);

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int position = 0; position < (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH);)
    {
        for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
        {
            for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
            {
                pixels[position].x_pos = x;
                pixels[position].y_pos = y;
                pixels[position].empty = true;
                pixels[position].r = 0;
                pixels[position].g = 0;
                pixels[position].b = 0;
                pixels[position].a = 255;
                position++;
            }
        }
    }

    // Draw it
    draw_and_render();

    std::cout << "Startup complete, there should be a black window displaying" << '\n';

    // It draws a box, and says it did
    draw_box(126, 130, 25, 35, 255, 255, 0, 255);
    draw_box(40, 100, 100, 200, 255, 255, 0, 255);
    draw_box(200, 240, 3, 5, 255, 255, 0, 255);
    draw_box(220, 226, 100, 200, 255, 255, 0, 255);

    std::cout << "Drew the box" << '\n';
    // Waits for 3 seconds
    SDL_Delay(3000);
    // Does the sand stuff
    for (int itteration = 0; itteration < 1000; itteration++)
    {
        sand_sim();
    }
    draw_and_render();
    std::cout << "Done the sim" << '\n';
    // Wait till ordered to kill the process, and free everything from the shackles of this computer world
    excecution_finished();
    return EXIT_SUCCESS;
}
void draw_and_render()
{
    for (int position = 0; position < (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position++)
    {
        SDL_SetRenderDrawColor(renderer, pixels[position].r, pixels[position].g, pixels[position].b, pixels[position].a);
        SDL_RenderDrawPoint(renderer, pixels[position].x_pos, pixels[position].y_pos);
    }
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
    delete[] pixels;
}
void draw_box(int x_range_s, int x_range_x, int y_range_s, int y_range_x, int r_val, int g_val, int b_val, int a_val)
{
    for (int position = 0; position < (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position++)
    {
        if (x_range_s < pixels[position].x_pos && pixels[position].x_pos < x_range_x)
        {
            if (y_range_s < pixels[position].y_pos && pixels[position].y_pos < y_range_x)
            {
                pixels[position].r = r_val;
                pixels[position].g = g_val;
                pixels[position].b = b_val;
                pixels[position].a = a_val;
                pixels[position].empty = false;
            }
        }
    }
    draw_and_render();
}
void sand_sim()
{
    for (int position = (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position > 0; position--)
    {
        if (pixels[position + LOGICAL_WINDOW_WIDTH].empty == true && pixels[position].y_pos != 256 && pixels[position].empty == false)
        {
            pixels[position + LOGICAL_WINDOW_WIDTH].r = pixels[position].r;
            pixels[position + LOGICAL_WINDOW_WIDTH].g = pixels[position].g;
            pixels[position + LOGICAL_WINDOW_WIDTH].b = pixels[position].b;
            pixels[position + LOGICAL_WINDOW_WIDTH].empty = pixels[position].empty;
            pixels[position].r = 0;
            pixels[position].g = 0;
            pixels[position].b = 0;
            pixels[position].empty = true;
            std::cout << "And another one bites the dust" << '\n';
        }
        else if (pixels[position + LOGICAL_WINDOW_WIDTH - 1].empty == true && pixels[position].y_pos != 256 && pixels[position].x_pos != 0 && pixels[position].empty == false)
        {
            pixels[position + LOGICAL_WINDOW_WIDTH - 1].r = pixels[position].r;
            pixels[position + LOGICAL_WINDOW_WIDTH - 1].g = pixels[position].g;
            pixels[position + LOGICAL_WINDOW_WIDTH - 1].b = pixels[position].b;
            pixels[position + LOGICAL_WINDOW_WIDTH - 1].empty = pixels[position].empty;
            pixels[position].r = 0;
            pixels[position].g = 0;
            pixels[position].b = 0;
            pixels[position].empty = true;
            std::cout << "And another one bites the dust" << '\n';
        }
        else if (pixels[position + LOGICAL_WINDOW_WIDTH + 1].empty == true && pixels[position].y_pos != 256 && pixels[position].x_pos != 256 && pixels[position].empty == false)
        {
            pixels[position + LOGICAL_WINDOW_WIDTH + 1].r = pixels[position].r;
            pixels[position + LOGICAL_WINDOW_WIDTH + 1].g = pixels[position].g;
            pixels[position + LOGICAL_WINDOW_WIDTH + 1].b = pixels[position].b;
            pixels[position + LOGICAL_WINDOW_WIDTH + 1].empty = pixels[position].empty;
            pixels[position].r = 0;
            pixels[position].g = 0;
            pixels[position].b = 0;
            pixels[position].empty = true;
            std::cout << "And another one bites the dust" << '\n';
        }
    }
    draw_and_render();
    std::cout << "NEXT ITERATION IN 1 SECOND PLEASE STANDBY" << '\n';
    SDL_Delay(50);
}


