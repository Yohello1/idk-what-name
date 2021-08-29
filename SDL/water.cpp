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
struct positions
{
    int y_pos;
    int x_pos;
    pixel_state state_now;
    int r;
    int g;
    int b;
    int a;
};
// Array of pixel stuff

auto pixels = std::make_unique<positions[]>(LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH);
auto new_pos = std::make_unique<positions[]>(LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH);

void draw_and_render(void);
void excecution_finished(void);
// x ranges, y ranges, and rgba colour
// s = smaller, x = larger
void draw_box(int x_range_s, int x_range_x, int y_range_s, int y_range_x, int r_val, int g_val, int b_val, int a_val, pixel_state new_state);
void fluid_sim(int test_amt_req);
void switch_arr();
void draw_and_render_debug();
void rgba(int position, int offset);

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
                new_pos[position].x_pos = pixels[position].x_pos = x;
                new_pos[position].y_pos = pixels[position].y_pos = y;
                pixels[position].r = 0;
                pixels[position].g = 0;
                pixels[position].b = 0;
                pixels[position].a = 255;
                pixels[position].state_now = empty;
                position++;
            }
        }
    }

    // Draw it
    draw_and_render();

    std::cout << "Startup complete, there should be a black window displaying" << std::endl;

    // It draws a box, and says it did
    draw_box(126, 130, 25, 35, 0, 95, 137, 255, fluid);
    draw_box(40, 100, 100, 200, 0, 95, 137, 255, fluid);
    draw_box(200, 240, 3, 5, 0, 95, 137, 255, fluid);
    draw_box(220, 226, 100, 200, 0, 95, 137, 255, fluid);
    draw_box(1, 256, 3, 5, 0, 95, 137, 255, fluid);

    std::cout << "Drew the box" << std::endl;
    // Waits for 3 seconds
    SDL_Delay(3000);
    // Does the sand stuff
    std::cout << "Start the thingy" << std::endl;

    fluid_sim(500);

    draw_and_render();
    std::cout << "Done the sim" << std::endl;
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
}
void draw_box(int x_range_s, int x_range_x, int y_range_s, int y_range_x, int r_val, int g_val, int b_val, int a_val, pixel_state new_state)
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
                pixels[position].state_now = new_state;
            }
        }
    }
    draw_and_render();
}

void fluid_sim(int test_amt_req)
{
    for (int tests = 0; tests < 10000; tests++)
    {
        for (int position = (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position > 0; position--)
        {
            if (pixels[position].state_now == fluid)
            {
                if (pixels[position].y_pos != LOGICAL_WINDOW_WIDTH - 1 && pixels[position + LOGICAL_WINDOW_WIDTH].state_now == empty && new_pos[position + LOGICAL_WINDOW_WIDTH].state_now == empty)
                {
                    // pixels[position + LOGICAL_WINDOW_WIDTH].state_now = pixels[position].state_now;
                    // pixels[position + LOGICAL_WINDOW_WIDTH].r = pixels[position].r;
                    // pixels[position + LOGICAL_WINDOW_WIDTH].g = pixels[position].g;
                    // pixels[position + LOGICAL_WINDOW_WIDTH].b = pixels[position].b;
                    // pixels[position + LOGICAL_WINDOW_WIDTH].a = pixels[position].a;

                    new_pos[position + LOGICAL_WINDOW_WIDTH].state_now = pixels[position].state_now;
                    new_pos[position + LOGICAL_WINDOW_WIDTH].r = pixels[position].r;
                    new_pos[position + LOGICAL_WINDOW_WIDTH].g = pixels[position].g;
                    new_pos[position + LOGICAL_WINDOW_WIDTH].b = pixels[position].b;
                    new_pos[position + LOGICAL_WINDOW_WIDTH].a = pixels[position].a;

                    new_pos[position].state_now = pixels[position].state_now = empty;
                    new_pos[position].r = pixels[position].r = 0;
                    new_pos[position].g = pixels[position].g = 0;
                    new_pos[position].b = pixels[position].b = 0;
                    new_pos[position].a = pixels[position].a = 0;
                }
                else if (pixels[position + LOGICAL_WINDOW_WIDTH - 1].state_now == empty && pixels[position].y_pos != (LOGICAL_WINDOW_WIDTH - 1) && pixels[position].x_pos != 1 && new_pos[position - 1 + LOGICAL_WINDOW_WIDTH].state_now == empty)
                {
                    // std::cout << "Another valid move" << std::endl;
                    // pixels[position + LOGICAL_WINDOW_WIDTH - 1].state_now = pixels[position].state_now;
                    // pixels[position + LOGICAL_WINDOW_WIDTH - 1].r = pixels[position].r;
                    // pixels[position + LOGICAL_WINDOW_WIDTH - 1].g = pixels[position].g;
                    // pixels[position + LOGICAL_WINDOW_WIDTH - 1].b = pixels[position].b;
                    // pixels[position + LOGICAL_WINDOW_WIDTH - 1].a = pixels[position].a;

                    new_pos[position + LOGICAL_WINDOW_WIDTH - 1].state_now = pixels[position].state_now;
                    new_pos[position + LOGICAL_WINDOW_WIDTH - 1].r = pixels[position].r;
                    new_pos[position + LOGICAL_WINDOW_WIDTH - 1].g = pixels[position].g;
                    new_pos[position + LOGICAL_WINDOW_WIDTH - 1].b = pixels[position].b;
                    new_pos[position + LOGICAL_WINDOW_WIDTH - 1].a = pixels[position].a;

                    // pixels[position].state_now = empty;
                    // pixels[position].r = 0;
                    // pixels[position].g = 0;
                    // pixels[position].b = 0;
                    // pixels[position].a = 0;

                    new_pos[position].state_now = pixels[position].state_now = empty;
                    new_pos[position].r = pixels[position].r = 0;
                    new_pos[position].g = pixels[position].g = 0;
                    new_pos[position].b = pixels[position].b = 0;
                    new_pos[position].a = pixels[position].a = 0;
                }
                else if (pixels[position + LOGICAL_WINDOW_WIDTH + 1].state_now == empty && pixels[position].y_pos != (LOGICAL_WINDOW_WIDTH - 1) && new_pos[position + 1 + LOGICAL_WINDOW_WIDTH].state_now == empty)
                {
                    // std::cout << "Another valid move" << std::endl;
                    // pixels[position + LOGICAL_WINDOW_WIDTH + 1].state_now = pixels[position].state_now;
                    // pixels[position + LOGICAL_WINDOW_WIDTH + 1].r = pixels[position].r;
                    // pixels[position + LOGICAL_WINDOW_WIDTH + 1].g = pixels[position].g;
                    // pixels[position + LOGICAL_WINDOW_WIDTH + 1].b = pixels[position].b;
                    // pixels[position + LOGICAL_WINDOW_WIDTH + 1].a = pixels[position].a;

                    new_pos[position + LOGICAL_WINDOW_WIDTH + 1].state_now = pixels[position].state_now;
                    new_pos[position + LOGICAL_WINDOW_WIDTH + 1].r = pixels[position].r;
                    new_pos[position + LOGICAL_WINDOW_WIDTH + 1].g = pixels[position].g;
                    new_pos[position + LOGICAL_WINDOW_WIDTH + 1].b = pixels[position].b;
                    new_pos[position + LOGICAL_WINDOW_WIDTH + 1].a = pixels[position].a;

                    // pixels[position].state_now = empty;
                    // pixels[position].r = 0;
                    // pixels[position].g = 0;
                    // pixels[position].b = 0;
                    // pixels[position].a = 0;

                    new_pos[position].state_now = pixels[position].state_now = empty;
                    new_pos[position].r = pixels[position].r = 0;
                    new_pos[position].g = pixels[position].g = 0;
                    new_pos[position].b = pixels[position].b = 0;
                    new_pos[position].a = pixels[position].a = 0;
                }

                if (pixels[position + 1].state_now == empty && pixels[position].x_pos != 255 && new_pos[position + 1].state_now == empty)
                {
                    std::cout << "Should move" << std::endl;
                    new_pos[position + 1].state_now = pixels[position].state_now;
                    new_pos[position + 1].r = pixels[position].r;
                    new_pos[position + 1].g = pixels[position].g;
                    new_pos[position + 1].b = pixels[position].b;
                    new_pos[position + 1].a = pixels[position].a;

                    new_pos[position].state_now = empty;
                    new_pos[position].r = 0;
                    new_pos[position].g = 0;
                    new_pos[position].b = 0;
                    new_pos[position].a = 0;
                    // switch_arr();
                    // SDL_Delay(1);
                    // draw_and_render_debug();
                }

                else
                {
                    new_pos[position].state_now = pixels[position].state_now;
                    new_pos[position].r = pixels[position].r;
                    new_pos[position].g = pixels[position].g;
                    new_pos[position].b = pixels[position].b;
                    new_pos[position].a = pixels[position].a;
                }
            }
        }
        switch_arr();
        draw_and_render_debug();
        std::cout << "NEXT ITERATION PLEASE WAIT" << std::endl;
    }
    draw_and_render();
    SDL_Delay(50);
}

void switch_arr()
{
    for (int position = (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position > 0; position--)
    {
        pixels[position].state_now = new_pos[position].state_now;
        pixels[position].r = new_pos[position].r;
        pixels[position].g = new_pos[position].g;
        pixels[position].b = new_pos[position].b;
        pixels[position].a = new_pos[position].a;

        if (pixels[position].r != new_pos[position].r || pixels[position].state_now != new_pos[position].state_now)
        {
            std::cout << "We got a problem houston" << std::endl;
        }
    }
}

void draw_and_render_debug()
{
    for (int position = 0; position < (LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH); position++)
    {
        SDL_SetRenderDrawColor(renderer, new_pos[position].r, new_pos[position].g, new_pos[position].b, new_pos[position].a);
        SDL_RenderDrawPoint(renderer, new_pos[position].x_pos, new_pos[position].y_pos);
    }
    SDL_RenderPresent(renderer);
}

void rgba(void)
{
    new_pos[position].state_now = pixels[position].state_now;
    new_pos[position].r =         pixels[position].r;
    new_pos[position].g =         pixels[position].g;
    new_pos[position].b =         pixels[position].b;
    new_pos[position].a =         pixels[position].a;
}