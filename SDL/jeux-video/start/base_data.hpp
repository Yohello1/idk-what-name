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

struct colour
{
    uint8_t r, g, b, a;
};
class cell
{
public:
    void reset_state()
    {
        pressure = 0;
        density = 0;
        temperature = 0;
    }

    // Function to return whether it can be interacted with or not
    // I dunno why Im making this a function lmao
    bool is_inert()
    {
        return inert;
    }

    /**
     * @brief changes the value of 2 cells, will only happen if possible, subtracts from first, adds to second
     *
     * @param orignal_ptr
     * @param succesor
     * @param change
     */
    void modify_pressure_2(cell *orignal_ptr, cell *succesor, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).pressure - change) > 1 && ((*succesor).pressure + change) < 65534)
        {
            // if (((*orignal_ptr).pressure - change) == 0)
            // {
            //     std::cout << "this is not possible" << '\n';
            // }
            (*orignal_ptr).pressure -= change;
            (*succesor).pressure += change;
        }
    }

    /**
     * @brief will subtract from one cell, subtract if possible
     *
     * @param orignal_ptr
     * @param succesor
     * @param change
     */
    void sub_pressure_if_possible(cell *orignal_ptr, cell *succesor, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).pressure - change) > 1)
        {
            // if (((*orignal_ptr).pressure - change) == 0)
            // {
            //     std::cout << "this is not possible" << '\n';
            // }
            (*orignal_ptr).pressure -= change;
            // (*succesor).pressure += change;
        }
    }

        /**
     * @brief First val is current cell, second is the old version of the one presure ismoving to, third is the new vresion where the pressure is moving to NOTE: CHANGE CAP
     *
     * @param orignal_ptr
     * @param succesor
     * @param predecessor
     * @param change
     */
    void modify_cross_die_pressure(cell *origin_pressure, cell *desination_soon, cell *to_be_added, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;

        // int previous_pressure = (*origin_pressure).fetch_pressure() + (*desination_soon).fetch_pressure() + (*to_be_added).fetch_pressure();

        // if (((*origin_pressure).fetch_pressure() - change) > 1 && ((*predecessor).fetch_pressure() + (*succesor).fetch_pressure() + change) < 500)
        // {
        //     (*orignal_ptr).pressure -= change;
        //     (*succesor).pressure += change;
        //     if (previous_pressure != (*orignal_ptr).fetch_pressure() + (*predecessor).fetch_pressure() + (*succesor).fetch_pressure())
        //     {
        //         std::cout << "this is not possible" << '\n';
        //     }
        // }
    
        if(origin_pressure->pressure - change > 0 && desination_soon->pressure + to_be_added->pressure + change < 500)
        {
            origin_pressure->pressure -= 1;
            //desination_soon->pressure += 1;
            to_be_added->pressure+=1;
        }
    }



    /**
     * @brief changes the value of one cell, just add
     *
     * @param orignal_ptr
     * @param change
     */
    void modify_pressure(cell *orignal_ptr, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).pressure - change) > 0)
        {
            (*orignal_ptr).pressure -= change;
        }
    }

    void modify_pressure_a(cell *orignal_ptr, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).pressure + change) < 65535)
        {
            (*orignal_ptr).pressure += change;
        }
    }

    /**
     * @brief changes the value of 2 cells, will only happen if possible, subtracts from first, adds to second
     *
     * @param orignal_ptr
     * @param succesor
     * @param change
     */
    void modify_density_2(cell *orignal_ptr, cell *succesor, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).density - change) > 1 && ((*succesor).density + change) < 65534)
        {
            if (((*orignal_ptr).density - change) == 0)
            {
                std::cout << "this is not possible" << '\n';
            }
            (*orignal_ptr).density -= change;
            (*succesor).density += change;
        }
    }

    /**
     * @brief changes the value of one cell, just add
     *
     * @param orignal_ptr
     * @param change
     */
    void modify_density(cell *orignal_ptr, int16_t change)
    {
        // cell* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).density - change) > 0)
        {
            (*orignal_ptr).density -= change;
        }
    }

    /*
    I wonder if Im driving away my friends, I hope im not
    */

    /**
     * @brief change it's colour, takes the colour construct as input
     *
     * @param input
     */
    void change_colour(colour input)
    {
        r = input.r;
        g = input.g;
        b = input.b;
        a = input.a;
    }
    /*
    I wonder if I'll ever find someone I can take my mask off infront of
    I bet it'll never happen
    */

    /**
     * @brief change pressure
     *
     * @param change
     */
    int16_t pressure_change(int16_t change)
    {
        pressure += change;

        return pressure;
    }

    /**
     * @brief change density
     *
     * @param change
     */
    int16_t density_change(uint16_t change)
    {
        density += change;

        return density;
    }

    /**
     * @brief change temperature
     *
     * @param new_temp
     */
    float temperature_change(float new_temp)
    {
        temperature = new_temp;

        return temperature;
    }

    /**
     * @brief
     *
     * @return int16_t
     */
    int16_t fetch_pressure()
    {
        return pressure;
    }

    /**
     * @brief
     *
     * @return int16_t
     */
    int16_t fetch_density()
    {
        return density;
    }

    /**
     * @brief Set the pressure object
     *
     * @param new_val
     */
    void set_pressure(uint16_t new_val)
    {
        pressure = new_val;
    }

    /**
     * @brief
     *
     * @return uint8_t
     */
    uint8_t fetch_r()
    {
        return r;
    }
    /**
     * @brief
     *
     * @return uint8_t
     */
    uint8_t fetch_g()
    {
        return g;
    }
    /**
     * @brief
     *
     * @return uint8_t
     */
    uint8_t fetch_b()
    {
        return b;
    }
    /**
     * @brief
     *
     * @return uint8_t
     */
    uint8_t fetch_a()
    {
        return a;
    }

    /**
     * @brief
     *
     * @param new_val
     */
    void change_r(uint8_t new_val)
    {
        r = new_val;
    }
    /**
     * @brief
     *
     * @param new_val
     */
    void change_g(uint8_t new_val)
    {
        g = new_val;
    }
    /**
     * @brief
     *
     * @param new_val
     */
    void change_b(uint8_t new_val)
    {
        b = new_val;
    }
    /**
     * @brief
     *
     * @param new_val
     */
    void change_a(uint8_t new_val)
    {
        a = new_val;
    }

    /**
     * @brief
     *
     * @return float
     */
    float fetch_temp()
    {
        return temperature;
    }

    /**
     * @brief It just SETS IT, does not add or subtract
     * 
     * @param new_temp 
     */
    void modify_temp(float new_temp)
    {
        temperature = new_temp;
    }

    /**
     * @brief 
     * 
     * @return uint8_t 
     */
    uint8_t fetch_flow()
    {
        return flow;
    }

    /**
     * @brief Set the flow object, JUST SETS NO ADD NO SUBTRACT
     * 
     * @param new_val 
     */
    void set_flow(uint8_t new_val)
    {
        flow = new_val;
    }

    /**
     * @brief SETS DENSITY
     * 
     * @param new_val 
     */
    void modify_density(uint16_t new_val)
    {
        density = new_val;
    }

    void set_inert(bool new_val)
    {
        inert = new_val;
    }

    bool get_empty()
    {
        return false;
    }
private:
    uint16_t pressure;
    uint16_t density;
    float temperature;
    uint8_t r, g, b, a;

    // Flow is 0 for solid stuff?
    uint8_t flow;
    bool inert = false;
    bool empty = false;

protected:
};

struct cord_2d
{
    u_int16_t x_pos;
    u_int16_t y_pos;
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
            pixels[x_pos][y_pos].change_a(0);
            pixels[x_pos][y_pos].change_g(0);
            pixels[x_pos][y_pos].change_b(0);
            pixels[x_pos][y_pos].change_r(0);
            pixels[x_pos][y_pos].modify_density(0);
            pixels[x_pos][y_pos].set_pressure(0);
            pixels[x_pos][y_pos].modify_temp(0);
            pixels[x_pos][y_pos].set_flow(0);
            pixels[x_pos][y_pos].set_inert(false);
        }
    }
}