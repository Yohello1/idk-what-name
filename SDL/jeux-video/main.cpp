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
#include "generation/terain.hpp"
#include "debug/printing.hpp"

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
    init::start();
    std::cout << "Started, allocating memory" << '\n';

    init::array_clean_start(pixels);
    // cord_2d box_verts[2];
    // box_verts[0].x_pos = 100;
    // box_verts[1].x_pos = 50;
    // box_verts[0].y_pos = 1;
    // box_verts[1].y_pos = 150;
    // draw_box_white_sand(box_verts[0],box_verts[1], pixels);

    const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_WIDTH;

    std::cout << "Thingy started now" << '\n';

    Terrain::Development Terrain_gen;

    Terrain_gen.Development::generate_terrain(0,LOGICAL_WINDOW_WIDTH/2,LOGICAL_WINDOW_WIDTH,LOGICAL_WINDOW_WIDTH,8,pixels);

    bool quit = false;
    std::thread physics(Physics::simulate, pixels,new_version, quit);
    // int frame_count = 0;
    while (!quit)
    {
        // auto start_time = Clock::now();
        // Debug_Printing::print_density(pixels);
        // std::cout << '\n';
        quit = Input_Large::poll_usr_input(changed, usr_input,&event,quit, actual_2_logic_ratio);

        mtx2.lock();
        Input_Large::mix_new_version_usr_input(changed, usr_input,pixels );
        std::memcpy(&render, &pixels, sizeof(pixels));
        mtx2.unlock();

		std::cout << "Quit Status " << quit << '\n';


        Render::redraw_render(render, renderer);
        SDL_RenderPresent(renderer);
        // auto end_time = Clock::now();

    }
    // Debug_Printing::print_density(pixels);
    physics.join();
    std::cout << "ENDED" << '\n';
    // std::cout << "Sand: " << Testing::count_sand(pixels) << '\n';
    excecution_finished();

}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

