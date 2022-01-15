#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

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

struct position pixel_pos[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
double ratio = ACTUAL_WINDOW_WIDTH/LOGICAL_WINDOW_WIDTH;
void excecution_finished();

// What the geunine duck is going on here
// I am going to pull of so mcuh mental gymnastics to get this workign LOL

// Cinamon toast here to explain wtf is going on
// *magic*, but actually
// First of all, make a texture, delete the texture data, but keep the pointer and allocated memory
// Now you are the texture
// // ~~What I mean is, you have to do the math for scalling...~~
// That was wrong, I just can't do math, and wasn't putting LOGICAL_WINDOW_WIDTH in the right places
// Because we literally copy paste the texture into the memory buffer
// So we do all this shid ourselves, may god have mercy upon my soul
int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << '\n';
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    Uint32 *pixels = new Uint32[LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH];
    memset(pixels, 128, LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH * sizeof(Uint32));

        bool leftMouseButtonDown = false;


    // PAINT IT BLACK
    while (1)
    {
        SDL_UpdateTexture(texture, NULL, pixels, LOGICAL_WINDOW_WIDTH * sizeof(Uint32));
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }

        switch (event.type)
        {
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = true;
        case SDL_MOUSEMOTION:
            if (leftMouseButtonDown)
            {
                int mouseX = event.motion.x* ratio;
                int mouseY = event.motion.y*ratio;
                pixels[mouseY * LOGICAL_WINDOW_WIDTH + mouseX] = 0;
            }
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Ending stuff
    delete[] pixels;
    SDL_DestroyTexture(texture);
    excecution_finished();
}

// idk what to name these functions

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
}
