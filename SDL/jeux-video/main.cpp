// I don't know why some of these libs are included by at this point Im too scared to remove them
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

// Including the files, will probs split this up even more at some point
#include "init_funcs.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "input.hpp"
#include "rand_testing.hpp"

void excecution_finished();

int main()
{

    // it starts it...
    // WHY DO YOU NEED A COMMENT TO UNDERSTAND THIS
    start();

    // Makes a ton of arrayes to be used later
    struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    struct position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    struct position ui[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    struct position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
     
    // This one is related to use input
    bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];


    // Sets everything to 0
    array_clean_start(pixels);

    // Makes a box
    cord_2d box_verts[2];
    box_verts[0].x_pos = 50;
    box_verts[1].x_pos = 40;
    box_verts[0].y_pos = 30;
    box_verts[1].y_pos = 50;
    draw_box_white_sand(box_verts[0],box_verts[1], pixels);

    // Does some calcs which will be used in a lot of other calcs
    int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_WIDTH;

    // is it time to quit?
    bool quit = false;
    
    // Open a thread for this
    std::thread physics(simulate, pixels,new_version, quit);
    // int frame_count = 0;
    while (!quit)
    {
        // I think I finally know what `auto` means

        // uh it's complicated, basically polls user input
        quit = poll_usr_input(changed, usr_input,&event, actual_2_logic_ratio);

        // Mixes the usr input and the new version. I should probs put this in inside the user iput thing
        mix_new_version_usr_input(changed, usr_input,pixels );

        // Copies it over
        std::memcpy(&render, &pixels, sizeof(pixels));

        // Renders it
        redraw_render(render, renderer);
        SDL_RenderPresent(renderer);

    }
    // Kills the child
    physics.join();

    // Ends it all
    std::cout << "ENDED" << std::endl;
    excecution_finished();

}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
