#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

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
    fire,
    burning,
    burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    uint8_t  r;
    uint8_t  g;
    uint8_t  b;
    uint8_t  a;
    uint16_t temperature;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();

// this script is gonn hurtme

int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << std::endl;
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    // PAINT IT BLACK

    excecution_finished();
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
