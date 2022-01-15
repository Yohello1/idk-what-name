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
            if (event->key.keysym.mod & KMOD_SHIFT)
            {
                shift = !shift;
            }
            break;
        case SDL_KEYUP:
            shift = !shift;
            std::cout << "hi" << '\n';
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

            for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
            {
                for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                {
                    usr_input[x_pos][y_pos].r = 148;
                    usr_input[x_pos][y_pos].g = 204;
                    usr_input[x_pos][y_pos].b = 222;
                    usr_input[x_pos][y_pos].a = 255;
                    usr_input[x_pos][y_pos].state_now = solid;
                    changed[x_pos][y_pos] = true;
                }
            }
        }
        else if (right_down == true && shift == false)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
            {
                for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                {
                    usr_input[x_pos][y_pos].r = 153;
                    usr_input[x_pos][y_pos].g = 0;
                    usr_input[x_pos][y_pos].b = 0;
                    usr_input[x_pos][y_pos].a = 255;
                    usr_input[x_pos][y_pos].state_now = solid;
                    changed[x_pos][y_pos] = true;
                }
            }
            // std::cout << "Right" << '\n';
        }
        else if (right_down == true && shift == true)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
            {
                for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                {
                    usr_input[x_pos][y_pos].r = 241;
                    usr_input[x_pos][y_pos].g = 27;
                    usr_input[x_pos][y_pos].b = 21;
                    usr_input[x_pos][y_pos].a = 255;
                    usr_input[x_pos][y_pos].state_now = fixed_pos;
                    changed[x_pos][y_pos] = true;
                }
            }
        }
        else if (left_down == true && shift == true)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << '\n';
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 190;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 238;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 254;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
            usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fluid;
            changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            // std::cout << "fluid" << '\n';

            for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
            {
                for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                {
                    usr_input[x_pos][y_pos].r = 190;
                    usr_input[x_pos][y_pos].g = 238;
                    usr_input[x_pos][y_pos].b = 254;
                    usr_input[x_pos][y_pos].a = 255;
                    usr_input[x_pos][y_pos].state_now = fluid;
                    changed[x_pos][y_pos] = true;
                }
            }
        }
    }

    return quit;
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
