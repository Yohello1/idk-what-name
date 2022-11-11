// Polling for user input
// This is gonna be painful
// #include "base_data.hpp"

// keep track of if it's held down or not
namespace Input_Large
{
    // Vars to keep track of the state of these 3 things
    // TODO: MAKE THIS MODULAR AND EASILY EXPANDABLE
    bool left_down = false, right_down = false, shift = false;
    int mouse_x = 0;
    int mouse_y = 0;

    // not even god knows what this does
    void mouse_Press(SDL_MouseButtonEvent &mouse_thing)
    {
        // Left Mouse Button, Right mouse button, in case you forgot
        if (mouse_thing.button == SDL_BUTTON_LEFT)
        {
            // Literally just switched the state of it, like inverts it
            left_down = !left_down;
        }
        else if (mouse_thing.button == SDL_BUTTON_RIGHT)
        {
            // Literally just switched the state of it, like inverts it
            right_down = !right_down;
        }
    }

    bool poll_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Event *event, bool quit, int actual_2_logic_ratio)
    {
        // Failed experiement,
        // give up on your dreams and die - levi
        // stdin mouse_down = SDL_MOUSEBUTTONDOWN;
        // Still have no clue what data type it is ;-;

        while (SDL_PollEvent(event) != 0)
        {
            // This is not a struct nor an object, I have no clue what this is
            // Just point to it (using an error)
            // I hate switch cases as much as you do ;-;
            switch (event->type)
            {
            case SDL_QUIT:
#ifdef INPUT_DEBUG
                std::cout << "QUIT" << '\n';
#endif
                quit = true;
                break;
            // When I wrote this, only two people knew how this worked, me and god
            // Now only god knows
            case SDL_MOUSEBUTTONDOWN:
                // Figure 2
                // This runs the function mouse_Press, gives it
                // Access to data abt the button pressed,
                // Inside that function it changes its state
                mouse_Press(event->button);
                recheck = true;
                break;
            case SDL_MOUSEBUTTONUP:
                // Refer to figure 2
                mouse_Press(event->button);
                break;
            case SDL_KEYDOWN:
                // I have no clue why these are named like these
                // The naming hurts, me...
                // I SHOULD WRITE MY OWN INPUT THINGY
                // Anyways, this gets the shift
                // Checks if it's actually shift
                // then yeah
                // Oh yeah, `&` and `&&` are diff things
                // `&` is bitwise, meaning literal
                // `&&` is logical, so boolean math go BRRRRRRR
                if (event->key.keysym.mod & KMOD_SHIFT)
                {
                    shift = !shift;
                }
#ifdef INPUT_DEBUG
                std::cout << "KEY DOWN EVENT" << '\n';
#endif
                break;
            case SDL_KEYUP:
                // wtf
                // I dont know why or how
                shift = !shift;
#ifdef INPUT_DEBUG
                std::cout << "KEY UP EVENT" << '\n';
#endif
                break;
            default:
                break;
            }
            recheck = true;

            // This is all scuffed, im gonna make a proper way to look for inputs... later
            // Figure 1
            if (left_down == true && shift == false)
            {
                recheck = true;
                std::cout << recheck << "Status\n";
                // Fetch mouse state, or cords
                SDL_GetMouseState(&mouse_x, &mouse_y);
#ifdef INPUT_DEBUG
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << '\n';
#endif

                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].change_r(255);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].change_g(255);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].change_b(255);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].change_a(255);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].temperature_change(0);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].set_pressure(0);
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].set_inert(true);
                changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
            }
            else if (left_down == true && shift == true)
            {
                recheck = true;
                std::cout << recheck << "Status\n";
                // Fetch mouse state, or cords
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4;
                     y_pos != LOGICAL_WINDOW_WIDTH - 1 &&
                     y_pos < (mouse_y / actual_2_logic_ratio) + 4;
                     y_pos++)
                {
                    for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4;
                         x_pos != LOGICAL_WINDOW_WIDTH - 1 &&
                         x_pos < (mouse_x / actual_2_logic_ratio) + 4;
                         x_pos++)
                    {
                        // Changes colours, properites etc
                        usr_input[x_pos][y_pos].change_r(0);
                        usr_input[x_pos][y_pos].change_g(0);
                        usr_input[x_pos][y_pos].change_b(255);
                        usr_input[x_pos][y_pos].change_a(255);
                        usr_input[x_pos][y_pos].set_pressure(100);
                        usr_input[x_pos][y_pos].set_flow(0);
                        usr_input[x_pos][y_pos].set_inert(false);
                        // Tells us later on which one changed
                        changed[x_pos][y_pos] = true;
                    }
                }
            }
        }

        // i gotta change this
        // TODO SWITCH THIS TO THE ATOMIC
        return quit;
    }

    // Gonna throw this part onto the backburner for now whilst I figure out what the heck is going on
    void mix_new_version_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {

        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                if (changed[x_pos][y_pos] == true)
                {
                    // Doesn't work dumbass
                    new_version[x_pos][y_pos] = usr_input[x_pos][y_pos];

                    // Resets it to 0
                    usr_input[x_pos][y_pos].change_r(0);
                    usr_input[x_pos][y_pos].change_g(0);
                    usr_input[x_pos][y_pos].change_b(0);
                    usr_input[x_pos][y_pos].change_a(0);
                    usr_input[x_pos][y_pos].temperature_change(0);
                    usr_input[x_pos][y_pos].set_pressure(100);
                    changed[x_pos][y_pos] = false;
                }
            }
        }
    }
}
