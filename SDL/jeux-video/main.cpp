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

#include "init_funcs.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "input_large.hpp"
#include "rand_testing.hpp"

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position ui[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::mutex mtx2;


int main()
{
    std::cout << "Starting" << '\n';
    start();
    std::cout << "Started, allocating memory" << '\n';

    array_clean_start(pixels);
    cord_2d box_verts[2];
    box_verts[0].x_pos = 100;
    box_verts[1].x_pos = 50;
    box_verts[0].y_pos = 1;
    box_verts[1].y_pos = 150;
    // draw_box_white_sand(box_verts[0],box_verts[1], pixels);

    const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_WIDTH;

    std::cout << "Thingy started now" << '\n';

    bool quit = false;
    std::thread physics(simulate, pixels,new_version, quit);
    int frame_count = 0;
    while (!quit)
    {
        auto start_time = Clock::now();

        quit = poll_usr_input(changed, usr_input,&event,quit, actual_2_logic_ratio);

        mtx2.lock();
        mix_new_version_usr_input(changed, usr_input,pixels );
        std::memcpy(&render, &pixels, sizeof(pixels));
        mtx2.unlock();

        redraw_render(render, renderer);
        SDL_RenderPresent(renderer);
        auto end_time = Clock::now();

    }
    physics.join();
    std::cout << "ENDED" << std::endl;
    std::cout << "Sand: " << count_sand(pixels) << std::endl;
    excecution_finished();

}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

