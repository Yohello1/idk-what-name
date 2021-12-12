#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>

#include "init_funcs.hpp"
#include "physics.hpp"
#include "render.hpp"

void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

int main()
{

    start();
    struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
    cord_2d box_verts[2];
    box_verts[0].x_pos = 250;
    box_verts[1].x_pos = 138;
    box_verts[0].y_pos = 1;
    box_verts[1].y_pos = 150;
    draw_box_white_sand(box_verts[0],box_verts[1], pixels);


    int frame_count = 0;
    bool quit = false;
    while (!quit)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            quit = true;
        }
        frame_count++;
        std::cout << "Frame " << frame_count << std::endl;
        sand_sim(pixels);
        redraw_render(pixels, renderer);
        SDL_RenderPresent(renderer);
        // SDL_Delay(50);
    }

    excecution_finished();

}



