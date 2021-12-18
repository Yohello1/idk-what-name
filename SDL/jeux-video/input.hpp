// Polling for user input
// This is gonna be painful
// #include "base_data.hpp"
#include <SDL2/SDL.h>
void poll_usr_input(position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Event* event, bool quit, int actual_2_logic_ratio)
{
    if (SDL_PollEvent(event) && event->type == SDL_QUIT)
    {
        quit = true;
    }
    int mouse_x = 0;
    int mouse_y = 0;
    if(event->motion.state & SDL_BUTTON_LMASK == SDL_BUTTON_LEFT)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_GetMouseState(&mouse_x, &mouse_y);
        std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
        SDL_RenderDrawPoint(renderer, mouse_x / actual_2_logic_ratio, mouse_y / actual_2_logic_ratio);
        SDL_RenderPresent(renderer);
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
        SDL_RenderPresent(renderer);
        std::cout << "Left" << std::endl;
    }

    if(event->motion.state & SDL_BUTTON_RMASK == SDL_BUTTON_RIGHT)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_GetMouseState(&mouse_x, &mouse_y);
        std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
        SDL_RenderDrawPoint(renderer, mouse_x / actual_2_logic_ratio, mouse_y / actual_2_logic_ratio);
        SDL_RenderPresent(renderer);
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
        usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
        SDL_RenderPresent(renderer);
        std::cout << "Right" << std::endl;
    }
}

// Gonna throw this part onto the backburner for now whilst I figure out what the heck is going on
void mix_new_version_usr_input(position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{

}