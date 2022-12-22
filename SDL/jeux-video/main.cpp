#include "start/base_data.hpp"
#include "render/stb_truetype.h"
#include "math/vector_math.hpp"
// #include "physics/physics.hpp"
#include "physics/physics2.hpp"
#include "render/render.hpp"
// #include "input/input_large.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
// A crap ton of arrays to deal with everything

// Simulated array everything is being fed
cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
cell usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;
const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;


int main()
{
    std::cout << "Starting" << '\n';

    // Starting stuff
    srand(current_time);




    // init::start();

    array_clean_start(pixels);

    std::thread physics(physics::simulate, pixels, new_version);

    // For now
    // while(!quit_now)
    while (!glfwWindowShouldClose(w))
    {

    }
    quit_now = true;
    physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();

}

void excecution_finished(void)
{

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}
