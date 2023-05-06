#include "start/base_data.hpp"
#include "include/stb/stb_truetype.h"
#include "include/stb/stb_image.h"
#include "math/vector_math.hpp"
// #include "physics/physics2.hpp"
#include "render/render.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "start/update_funcs.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
#include "render/texture.hpp"

// Simulated array everything is being fed
cell_t pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
// This stuff's apparently never defined LOL
// struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
cell_t usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell_t render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;

int main()
{

}

void excecution_finished(void)
{

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}

/**
 * @brief Literally just meant to throw random data whether it be colour or properties into the array
 *
 */
void make_temp_arrays()
{
    for (int i = 0; i < LOGICAL_WINDOW_HEIGH; i++)
    {
        for (int j = 0; j < LOGICAL_WINDOW_WIDTH; j++)
        {
            colour_t temp;

            std::random_device dev;
            std::mt19937 rng(dev());

            // Random colours
            temp.r = (rand() % 255);
            temp.g = (rand() % 255);
            temp.b = (rand() % 255);
            temp.a = (rand() % 255);

            pixels[i][j].set_col(temp);
        }
    }
}
