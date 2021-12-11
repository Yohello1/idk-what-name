#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
SDL_MouseButtonEvent *button;

enum pixel_state
{
    empty,
    gas,
    fluid,
    sand,
    solid,
    fire,
    burning,
    burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint16_t temperature;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

std::mutex locks[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump_state();
void scr_dump_colour();

void redraw_and_render();
void excecution_finished();
// void sand_sim();

// Thread functions
// void polling_thread();
// void rendering_thread_function();

void fall();
/*
Im sorry threading functions, 
you were meant to be so much more
you were meant to save us
help us
but, the limitations of SDL
limits our abilities

ANYWAYS SIMULATION STUFF GO GO GO
WE HAVE HOSTILES INCOMING

SIR WE CANT HOLD EM

I SAID HOLD EM, WE JUST NEED TO HOLD ON FOR ANOTHER-
OH ODIN WHAT'S THAT

Dies
*/

// The ratios
const uint_fast8_t actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;

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

    // Now we need to get the ratio

    // PAINT IT BLACK
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    for (int x_pos = 0; x_pos < 0; x_pos++)
    {
        for (int y_pos = 0; y_pos < 0; y_pos++)
        {
            pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].temperature = 0;
            pixels[x_pos][y_pos].state_now = empty;
        }
    }

    // Joining the threads so I dont have to put down a revolution
    SDL_RenderPresent(renderer);

    // std::thread sand_pointer(fall);

    std::cout << "So the threads have started" << std::endl;
    bool quit = false;
    uint i = 0;
    int mouse_x = 0, mouse_y = 0;
    while (!quit)
    {
        i++;
        std::cout << "Itteration" << i << std::endl;
        while (SDL_PollEvent(&event) != 0)
        {
            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        // switch (event.button.button)
        // {
        // case SDL_BUTTON_RIGHT:
        if(event.motion.state & SDL_BUTTON_LMASK == SDL_BUTTON_LEFT)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            SDL_RenderDrawPoint(renderer, mouse_x / actual_2_logic_ratio, mouse_y / actual_2_logic_ratio);
            SDL_RenderPresent(renderer);
            pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
            pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
            SDL_RenderPresent(renderer);
            std::cout << "Left" << std::endl;
        }
        //     break;
        // case SDL_BUTTON_LEFT:
        if(event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            // set draw colour
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            // Get mouse position
            SDL_GetMouseState(&mouse_x, &mouse_y);
            //Output location
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            std::cout << "Right" << std::endl;
            // Get mouse position, convert to logical position, then make like a block around it which is 8x8 to make I think white
            for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
            {
                for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                {
                    // Drawing , outputing position, draw, and seting new state
                    SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                    std::cout << "(" << x_pos << "," << y_pos << ")" << std::endl;
                    pixels[x_pos][y_pos].state_now = solid;
                    pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].b = 255;
                }
            }
        }

        //     break;
        // }

        // Sand simulation
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
            {
                if (pixels[x_pos][y_pos].state_now == solid && y_pos != 255 && pixels[x_pos][y_pos + 1].state_now == empty)
                {

		  // std::cout << "Valid move" << std::endl;
                    pixels[x_pos][y_pos + 1] = pixels[x_pos][y_pos];
                    pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 0;
                    pixels[x_pos][y_pos].state_now = empty;
                }
                else if (pixels[x_pos][y_pos].state_now == solid && y_pos != 255 && pixels[x_pos + 1][y_pos + 1].state_now == empty)
                {

		  // std::cout << "Valid move" << std::endl;
                    pixels[x_pos + 1][y_pos + 1] = pixels[x_pos][y_pos];
                    pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 0;
                    pixels[x_pos][y_pos].state_now = empty;
                }
                else if (pixels[x_pos][y_pos].state_now == solid && y_pos != 255 && pixels[x_pos - 1][y_pos + 1].state_now == empty)
                {

		  // std::cout << "Valid move" << std::endl;
                    pixels[x_pos - 1][y_pos + 1] = pixels[x_pos][y_pos];
                    pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 0;
                    pixels[x_pos][y_pos].state_now = empty;
                }
            }
        }

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

    // sand_pointer.join();
    // So this is the reason it was being dumped LOL
    // scr_dump();
    // scr_dump_state();
    excecution_finished();
}

void fall()
{

    int i = 0;
    bool quit = false;
    while (!quit)
    {
        // check if it can die yet
        while (SDL_PollEvent(&event) != 0)
        {
            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        i++;
        std::cout << "Another sand falling down " << i << std::endl;

        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                locks[x_pos][y_pos].lock();
                locks[x_pos][y_pos + 1].lock();
                if (pixels[x_pos][y_pos + 1].state_now != fixed_pos && y_pos != 255 && pixels[x_pos][y_pos].state_now == solid)
                {
                    pixels[x_pos][y_pos + 1] = pixels[x_pos][y_pos];
                    pixels[x_pos][y_pos].r =
                        pixels[x_pos][y_pos].g =
                            pixels[x_pos][y_pos].b =
                                pixels[x_pos][y_pos].a = 0;
                    pixels[x_pos][y_pos].state_now = empty;
                    std::cout << "Hello" << std::endl;
                }
                locks[x_pos][y_pos].unlock();
                locks[x_pos][y_pos + 1].unlock();
            }
        }
    }
}

// Debug functions
void scr_dump_state()
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
void scr_dump_colour()
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            std::cout << "(" << pixels[x_pos][y_pos].r << pixels[x_pos][y_pos].g << pixels[x_pos][y_pos].b << ")";
        }
        std::cout << std::endl;
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

            if (pixels[x_pos][y_pos].state_now == solid)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }
}
void excecution_finished(void)
{
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
