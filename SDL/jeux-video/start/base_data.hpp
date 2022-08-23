#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <memory>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <set>
#include <vector>
#include <array>

#define STB_TRUETYPE_IMPLEMENTATION
#define BLEND_TEXT 0
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024
// #define INPUT_DEBUG

typedef std::chrono::high_resolution_clock Clock;
std::atomic<bool> quit_now;

// TODO: change the input system to properly check this stuff
std::atomic<bool> recheck;

// time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

// something needs to bedone about this
// Use density, and type (static or dynamic)
// entity is rlly just fixed_pos with extra steps
enum pixel_state
{
    empty,
    fluid,
    solid,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    // Yes I could do all these vars in one line, but this is "cleaner"
    uint8_t r, g, b, a;
    uint8_t density;
    uint8_t pressure;
    uint16_t temperature;
};

class cell
{
public:
    uint8_t r, g, b, a;
    uint8_t pressure;
    uint8_t density;
    uint8_t temperature;
    // Flow is 0 for solid stuff?
    uint8_t flow;
    bool inert = false;

    // Function to return whether it can be interacted with or not
    // I dunno why Im making this a function lmao
    bool is_inert()
    {
        return inert;
    }
private:
protected:
};

struct cord_2d
{
    u_int16_t x_pos;
    u_int16_t y_pos;
};

struct colour
{
    uint8_t r, g, b, a;
};

namespace init
{
    void start()
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        // Printing the time
        std::cout << "Time = " << current_time << "\n";
        // setting random seed
        srand(current_time);
        // rise my glorious creation*

        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
        SDL_SetWindowTitle(window, "Gamen't");
        SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
        SDL_RenderClear(renderer);
    }

    void array_clean_start(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = 0;
                pixels[x_pos][y_pos].state_now = empty;
                pixels[x_pos][y_pos].temperature = 0;
                pixels[x_pos][y_pos].pressure = 0;
                pixels[x_pos][y_pos].density = 0;
            }
        }
    }
}
