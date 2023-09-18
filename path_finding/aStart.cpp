#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define LOGICAL_WINDOW_WIDTH 256
#define PI 3.14159265
//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct pixel_data_struct
{
    // Maybe I should make this a cords 2d
    cord_2d pixel_parent;
    int HCost;
    int GCost;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
struct pixel_data_struct pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct pixel_data_struct new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump();
void redraw_and_render();
void excecution_finished();
int noise_gen();
// this script is gonn hurtme

// ion feel like taking usr input so these are just gonna be pre-defined global variables
const int noise_density = 90;
double wind_dir_degrees = 55;

int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << '\n';
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1024, 1024, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    // PAINT IT BLACK
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    // Give everything a default value
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = 0;
            pixels[x_pos][y_pos].pixel_parent = NULL;
        }
    }

    //Gonna make points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    std::cout << "Wait for 1 second pls" << '\n';
    SDL_Delay(1000);

    // draw fire
    for (int x_pos = 120; x_pos < 128; x_pos++)
    {
        for (int y_pos = 120; y_pos < 128; y_pos++)
        {
            pixels[x_pos][y_pos].r = 255;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
        }
    }
    // Noise
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            if (noise_gen() == 1)
            {
                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = 255;

                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                SDL_RenderPresent(renderer);
            }
        }
    }
    // redraw_and_render();
    // uh does this make sense?
    for (int i = 0; i > -1; i++)
    {
        std::cout << i << '\n';

        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
               // Write ur code here :3
            }
        }
        SDL_Delay(10);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;
        }
    }
    excecution_finished();
}

// Debug functions
void scr_dump()
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            // std::cout << pixels[x_pos][y_pos].state_now;
        }
        std::cout << '\n';
    }
}
void redraw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
        }
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
} //                                                                                                                                                                                                                                                                                                                                  v

int noise_gen()
{
    int num = (rand() % 100);

    if (num > noise_density)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}
