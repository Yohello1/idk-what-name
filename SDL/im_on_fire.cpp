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
enum pixel_state
{
    empty,
    gas,
    fluid,
    solid,
    fire,
    burning,
    burnt,
    eternal_fire,
    fixed_pos
};
enum flame
{
    flameable,
    non_flamable,
    external_fire,
    baguette
};
struct pixel_data_struct
{
    pixel_state state_now;
    flame flameability;
    int r;
    int g;
    int b;
    int a;
    int temperature;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct pixel_data_struct pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct pixel_data_struct new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump();
void redraw_and_render();
void excecution_finished();
int noise_gen();
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
    // Give everything a default value
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = 0;
            pixels[x_pos][y_pos].state_now = empty;
            pixels[x_pos][y_pos].temperature = 0;
            pixels[x_pos][y_pos].flameability = flameable;
        }
    }

    //Gonna make points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    std::cout << "Wait for 1 second pls" << std::endl;
    SDL_Delay(1000);

    // I gueinuely have no idea is gonna happen from now on
    for (int x_pos = 120; x_pos < 128; x_pos++)
    {
        for (int y_pos = 120; y_pos < 128; y_pos++)
        {
            pixels[x_pos][y_pos].r = 255;
            pixels[x_pos][y_pos].temperature = 100;
            pixels[x_pos][y_pos].state_now = burning;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
        }
    }
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            if (noise_gen() == 1)
            {
                pixels[x_pos][y_pos].flameability = non_flamable;
                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = 255;

                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                SDL_RenderPresent(renderer);
            }
        }
    }
    // redraw_and_render();
    // uh does this make sense?
    for (int i = 0; i < 1000; i++)
    {
        std::cout << i << std::endl;

        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                if (pixels[x_pos][y_pos].state_now == burning)
                {
                    // std::cout << "There is a toasty baguette here" << std::endl;
                    /*
                     . 3 .
                     2 X 4
                     . 1 .
                    */
                    if (pixels[x_pos][y_pos + 1].flameability == flameable && pixels[x_pos][y_pos + 1].temperature != 200)
                    {
                        pixels[x_pos][y_pos + 1].temperature += 1;
                        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].temperature * 2.5, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                    }
                    if (pixels[x_pos - 1][y_pos].flameability == flameable && pixels[x_pos - 1][y_pos].temperature != 200)
                    {
                        pixels[x_pos - 1][y_pos].temperature += 1;
                        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].temperature * 2.5, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                    }
                    if (pixels[x_pos][y_pos - 1].flameability == flameable && pixels[x_pos][y_pos - 1].temperature != 200)
                    {
                        pixels[x_pos][y_pos - 1].temperature += 1;
                        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].temperature * 2.5, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                    }
                    if (pixels[x_pos + 1][y_pos].flameability == flameable && pixels[x_pos + 1][y_pos].temperature != 200)
                    {
                        pixels[x_pos + 1][y_pos].temperature += 1;
                        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].temperature * 2.5, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);    
                    }
                }
                if (pixels[x_pos][y_pos].temperature == 20)
                {
                    pixels[x_pos][y_pos].state_now = burning;
                }
                if (pixels[x_pos][y_pos].temperature == 200)
                {
                    pixels[x_pos][y_pos].flameability = non_flamable;
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                    SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                }
            }
        }
        SDL_Delay(50);
        SDL_RenderPresent(renderer);
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
        }
    }
    SDL_RenderPresent(renderer);
}
void excecution_finished(void)
{
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
        SDL_RenderPresent(renderer);
        // SDL_Delay(50);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
} //                                                                                                                                                                                                                                                                                                                                  v

int noise_gen()
{
    int num = (rand() % 100);

    if (num > 80)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}