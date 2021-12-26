// Polling for user input
// This is gonna be painful
// #include "base_data.hpp"
#include <SDL2/SDL.h>

// keep track of if it's held down or not
bool left_down = false, right_down = false, shift = false;

// not even god knows what this does
void mouse_Press(SDL_MouseButtonEvent &mouse_thing)
{

    if (mouse_thing.button == SDL_BUTTON_LEFT)
    {
        left_down = !left_down;
    }
    else if (mouse_thing.button == SDL_BUTTON_RIGHT)
    {
        right_down = !right_down;
    }
}

bool poll_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Event *event, bool quit, int actual_2_logic_ratio)
{
    // Failed experiement,
    // give up on your dreams and die - levi
    // stdin mouse_down = SDL_MOUSEBUTTONDOWN;
    bool quit_now = false;
    int mouse_x = 0;
    int mouse_y = 0;
    while (SDL_PollEvent(event) != 0)
    {
        // Access the event type dumbass
        switch (event->type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_Press(event->button);
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_Press(event->button);
            break;
        case SDL_KEYDOWN:
            // L_shift = true;
            // std::cout << "I am a banana" << '\n';
            // if (event->key.keysym.sym == SDLK_a)
            // {
            //     std::cout << "A" << '\n';
            //     if (event->key.keysym.mod & KMOD_SHIFT)
            //     {
            //     std::cout << "A2" << '\n';
            //     }
            //     else
            //     {
            //         // Handle 'a'
            //     }
            // }
            if(event->key.keysym.mod & KMOD_SHIFT)
            {
                shift = !shift;
            }
            break;
        case SDL_KEYUP:
            // if(event->key.keysym.mod & KMOD_SHIFT)
            // {
                shift = !shift;
                std::cout << "hi" << '\n';
            // }
            std::cout << "hi2" << '\n';
            break;
        default:
            break;
        }

        int mouse_x = 0;
        int mouse_y = 0;
        if (left_down == true && shift == false)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 148;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 204;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 222;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
            changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            std::cout << "Left" << std::endl;
        }
        else if (right_down == true && shift == false)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 153;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 0;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
            changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            std::cout << "Right" << std::endl;
        }
        else if (right_down == true && shift == true)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 241;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 27;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 21;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
            changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            std::cout << "Fixed" << std::endl;
        }
        else if (left_down == true && shift == true)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 190;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 238;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 254;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fluid;    
            changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            std::cout << "fluid" << std::endl;
        }
    }

    return quit_now;
}

// Gonna throw this part onto the backburner for now whilst I figure out what the heck is going on
void mix_new_version_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            if (changed[x_pos][y_pos] == true)
            {
                // Doesn't work dumbass
                // new_version[x_pos][y_pos] = usr_input[x_pos][y_pos];
                new_version[x_pos][y_pos].r = usr_input[x_pos][y_pos].r;
                new_version[x_pos][y_pos].g = usr_input[x_pos][y_pos].g;
                new_version[x_pos][y_pos].b = usr_input[x_pos][y_pos].b;
                new_version[x_pos][y_pos].a = usr_input[x_pos][y_pos].a;
                new_version[x_pos][y_pos].state_now = usr_input[x_pos][y_pos].state_now;
                new_version[x_pos][y_pos].pressure = usr_input[x_pos][y_pos].pressure;
                new_version[x_pos][y_pos].temperature = usr_input[x_pos][y_pos].temperature;

                usr_input[x_pos][y_pos].r = usr_input[x_pos][y_pos].g = usr_input[x_pos][y_pos].b = usr_input[x_pos][y_pos].a = usr_input[x_pos][y_pos].temperature = usr_input[x_pos][y_pos].pressure = 0;
                usr_input[x_pos][y_pos].state_now = empty;
                changed[x_pos][y_pos] = false;
            }
        }
    }
}
