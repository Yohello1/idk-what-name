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
#include <boost/algorithm/clamp.hpp>
#include <algorithm>
#include <iomanip>
#include <SDL2/SDL.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define BLEND_TEXT 0
#define LOGICAL_WINDOW_WIDTH 128
#define ACTUAL_WINDOW_WIDTH 1024
#define INPUT_DEBUG

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
    float pressure;
    float density;
    float temperature;
    // Flow is 0 for solid stuff?
    uint8_t flow;
    bool inert = false;

    // Function to return whether it can be interacted with or not
    // I dunno why Im making this a function lmao
    bool is_inert()
    {
        return inert;
    }

    // If input2 minus change is below 0 it fais
    /**
     * @brief Adds `change`, to input1, and subtracts from input2
     * Fails if it will lower input 2 below 0
     *
     * @param input1
     * @param input2
     * @param change
     * @return true
     * @return false
     */
    bool add_and_remove(float input1, float input2, float change)
    {
        bool success = false;

        if ((input2 - change) > 0)
        {
            success = true;
            input1 += change;
            input2 -= change;
        }

        return success;
    }

    /**
     * @brief Adds a value to the input, but only if the result is bigger than 0
     * 
     * @param input 
     * @param change 
     * @return true 
     * @return false 
     */
    float add(float input, float change)
    {
        float value = input;

        if((input + change) > 0.001)
        {
            value += change;
        }

        return value;
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

}
void array_clean_start(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = 0;
            pixels[x_pos][y_pos].density = 0;
            pixels[x_pos][y_pos].pressure = 0;
            pixels[x_pos][y_pos].temperature = 0;
            pixels[x_pos][y_pos].flow = 0;
            pixels[x_pos][y_pos].inert = false;
        }
    }
}