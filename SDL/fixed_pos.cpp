#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define LOGICAL_WINDOW_WIDTH 256

//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
SDL_MouseButtonEvent *button;

enum pixel_state
{
    empty,
    gas,
    fluid,
    solid,
    fire,
    burning,
    burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint16_t temperature;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump();
void redraw_and_render();
void excecution_finished();

// this script is gonn hurtme

int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << std::endl;
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1024, 1024, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    // PAINT IT BLACK
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    for (int x_pos = 0; x_pos < 0; x_pos++)
    {
        for (int y_pos = 0; y_pos < 0; y_pos++)
        {
            pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].temperature = 0;
            pixels[x_pos][y_pos].state_now = empty;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int mouse_x = 0, mouse_y = 0;
    bool quit = false;
    bool mouse_down = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {

            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                mouse_down = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                mouse_down = false;
            }
            // if (mouse_down == true)
            // {
            //     if (event.button.button == SDL_BUTTON_LEFT)
            //     {
            //         SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            //         SDL_GetMouseState(&mouse_x, &mouse_y);
            //         std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
            //         SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
            //         SDL_RenderPresent(renderer);
            //         pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].g = pixels[mouse_x / 4][mouse_y / 4].a = 255;
            //     }
            //     if (event.button.button == SDL_BUTTON_RIGHT)
            //     {
            //         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //         SDL_GetMouseState(&mouse_x, &mouse_y);
            //         std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
            //         SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
            //         SDL_RenderPresent(renderer);
            //         pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].g = pixels[mouse_x / 4][mouse_y / 4].b = pixels[mouse_x / 4][mouse_y / 4].a = 255;
            //         // pixels[x_pos][y_pos].state_now = fixed_pos;
            //     }

            //     // pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 255;
            //     // pixels[x_pos][y_pos].state_now = fixed_pos;
            // }

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_GetMouseState(&mouse_x, &mouse_y);
                std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
                SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
                SDL_RenderPresent(renderer);
                pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].g = pixels[mouse_x / 4][mouse_y / 4].a = 255;
                break;
            case SDL_BUTTON_RIGHT:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_GetMouseState(&mouse_x, &mouse_y);
                std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
                SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
                SDL_RenderPresent(renderer);
                pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].g = pixels[mouse_x / 4][mouse_y / 4].b = pixels[mouse_x / 4][mouse_y / 4].a = 255;
                break;
            }
        }
        // if (event.type == SDL_MOUSEBUTTONDOWN)
        // {
        //     switch (event.button.button)
        //     {
        //     case SDL_BUTTON_LEFT:
        //         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //         SDL_GetMouseState(&mouse_x, &mouse_y);
        //         std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
        //         SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
        //         SDL_RenderPresent(renderer);
        //         pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].g = pixels[mouse_x / 4][mouse_y / 4].b = pixels[mouse_x / 4][mouse_y / 4].a = 255;
        //         break;
        //     case SDL_BUTTON_RIGHT:
        //         SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        //         pixels[mouse_x / 4][mouse_y / 4].r = pixels[mouse_x / 4][mouse_y / 4].b = pixels[mouse_x / 4][mouse_y / 4].a = 255;
        //         pixels[mouse_x / 4][mouse_y / 4].g = 0;
        //         SDL_GetMouseState(&mouse_x, &mouse_y);
        //         std::cout << "(" << mouse_x / 4 << "," << mouse_y / 4 << ")" << std::endl;
        //         SDL_RenderDrawPoint(renderer, mouse_x / 4, mouse_y / 4);
        //         SDL_RenderPresent(renderer);
        //         break;
        //     default:
        //         break;
        //     }
        // }
    }

    excecution_finished();
}

// Debug functions
void scr_dump()
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            std::cout << pixels[x_pos][y_pos].state_now;
        }
        std::cout << std::endl;
    }
}
void redraw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            if (pixels[x_pos][y_pos].b == 255)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }
}
void excecution_finished(void)
{

    SDL_RenderPresent(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
