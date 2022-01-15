#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <iostream>

#define LOGICAL_WINDOW_WIDTH 360
#define ACTUAL_WINDOW_WIDTH 1440

SDL_Renderer *renderer;
SDL_Window *window;

unsigned int current_time = (unsigned int)time(NULL);

void excecution_finished();

int main()
{
    // How have I forgotten to write code already I haven't even started
    std::cout << "Hello World" << '\n';

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Gamen't");
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);

    int actual_2_logic_ratio = LOGICAL_WINDOW_WIDTH / ACTUAL_WINDOW_WIDTH;
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while (!quit)
    {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }



        // Render
        for(int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH;y_pos++)
        {
            // Im just tired and don't want to work on this project anymore this is a waste of time
            // Why am I still working on this
        }









    }
    excecution_finished();
}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}