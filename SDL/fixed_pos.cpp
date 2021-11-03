#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 512

/*
Note to future self:
Fuck this shit Im out

Anyways,
For some reason splitting it into 2 different threads was FINE and somehow
didnt kill this via mutexs, deadlocks, and stuff????
So I might leave this as is, and race condtions lol
 */




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

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump();
void redraw_and_render();
void excecution_finished();
// void sand_sim();

// The ratios
const uint_fast8_t actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;

// Starting thread stuff
void rendering_thread(void);
void polling_thread(void);
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

    // Starting up the threads, one for rendering, another for polling, oh gosh something is gonna break
    // Wasnt I supposed to make a pointer??
    std::thread render_thread_pointer(rendering_thread);
    std::thread polling_thread_pointer(polling_thread);

    // Joining the threads so I dont have to put down a revolution
    polling_thread_pointer.join();
    render_thread_pointer.join();
    excecution_finished();
}

void polling_thread(void)
{
  int mouse_x = 0, mouse_y = 0;
  bool quit = false;
  int i = 0;
  while (!quit)
    {
      i++;
      std::cout << "Polling itteration" << i << std::endl;
        while (SDL_PollEvent(&event) != 0)
        {
            // This is gonna be annoying
            /*
                Make 2 threads, one for user input, other for simulation
                Why 2 threads?
                Cause doing this on one breaks soo muchhhh
            */

            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;

                break;
            case SDL_BUTTON_RIGHT:
                // set draw colour
                // Get mouse position
                SDL_GetMouseState(&mouse_x, &mouse_y);
                //Output location
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                // Get mouse position, convert to logical position, then make like a block around it which is 8x8 to make I think white
                for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
                {
                    for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                    {
                        // Drawing , outputing position, draw, and seting new state

                        std::cout << "(" << x_pos << "," << y_pos << ")" << std::endl;
                        pixels[x_pos][y_pos].state_now = solid;
                        pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].b = 255;
                    }
                }

                break;
            }
        }
    }
  
}


void rendering_thread(void)
{
    std::cout << "Thread Started" << std::endl;
    bool quit = false;
    int i = 0;
    while (!quit)
    {
      i++;
      std::cout << "Itteration rendering" << i << std::endl;
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {

            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(17);
        if (event.type == SDL_QUIT)
        {
            quit = true;
            break;
        }
    }
	std::cout << "Oh no What ever will I do" << std::endl;
}



// Debug functions
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
