#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>

#define LOGICAL_WINDOW_WIDTH 256
#define PI 3.14159265
//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

// why must I make this
struct cord_2d
{
    uint8_t x_pos;
    uint8_t y_pos;
};

struct aStarFinding
{
    cord_2d pixel_parent;
    uint16_t HCost;
    uint16_t GCost;
    uint16_t fCost;
};

struct pixel_data_struct
{
    bool inert = false;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct boids
{
    uint8_t direction;
    uint8_t velocity;
    cord_2d position;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    int path_find(cord_2d endPoint)
    {
        // https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2
        std::vector<unique_ptr>


    }
};

struct pixel_data_struct pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct boids birds[1024];
struct aStarFinding shadeOfBlue[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// another aStarFinding array needs to be made for each thread... but this is single threaded

void scr_dump();
void redraw_and_render();
void excecution_finished();
void draw_birds();
void reset_astar_array();
int noise_gen();
// this script is gonn hurtme
// I havent listened to kms - sub urban in a while
// ion feel like taking usr input so these are just gonna be pre-defined global variables
const int noise_density = 90;

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
            // pixels[x_pos][y_pos].pixel_parent = void;
        }
    }

    //Gonna make points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    std::cout << "Wait for 1 second pls" << '\n';
    SDL_Delay(1000);

    // Noise
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            if (noise_gen() == 1)
            {
                pixels[x_pos][y_pos].inert = true;
            }
        }
    }

    std::cout << "why is it not sending" << std::endl;

    for(int i = 0; i < (sizeof(birds))/(sizeof(uint8_t)*8); i++)
    {
        birds[i].r = (rand() % 255);
        birds[i].g = (rand() % 255);
        birds[i].b = (rand() % 255);
        birds[i].position.x_pos = (rand() % LOGICAL_WINDOW_WIDTH);
        birds[i].position.y_pos = (rand() % LOGICAL_WINDOW_WIDTH);

    }

    std::cout << "visual" << std::endl;
    // redraw_and_render();
    // uh does this make sense?
    bool kys = false;
    while(!kys)
    {

        // CODE HERE
        // Ok first of all, let's make the birds
        //

        redraw_and_render();

        SDL_Delay(10);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            kys = true;
            // SDL_DestroyRenderer(renderer);
            // SDL_DestroyWindow(window);
            // SDL_Quit();
            // return 0;
        }
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
            // std::cout << pixels[x_pos][y_pos].state_now;
        }
        std::cout << '\n';
    }
}
void redraw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            if(pixels[x_pos][y_pos].inert == true)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
        }
    }
    draw_birds();
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
void draw_birds()
{
    for(uint i = 0; i < (sizeof(birds))/(sizeof(uint8_t)*8); i++)
    {
        SDL_SetRenderDrawColor(renderer, birds[i].r, birds[i].g, birds[i].b, birds[i].a);
        SDL_RenderDrawPoint(renderer, birds[i].position.x_pos, birds[i].position.y_pos);
    }
}
void reset_astar_array()
{
    memset(&shadeOfBlue, 0, sizeof(shadeOfBlue));
}
void pathFindChildren(cord_2d endPoint)
{
    for(uint i = 0; i < (sizeof(birds))/(sizeof(uint8_t)*8); i++)
    {
        reset_astar_array();
        shadeOfBlue[birds[i].position.x_pos][birds[i].position.y_pos].GCost = 0;
        shadeOfBlue[birds[i].position.x_pos][birds[i].position.y_pos].HCost = 0;
        birds[i].path_find(endPoint);
    }

}

int noise_gen()
{
    int num = (rand() % 100);

    if (num > noise_density)
    {
        num = 1;
    }
    else
    {
        num = 0;
    }
    return num;
}
