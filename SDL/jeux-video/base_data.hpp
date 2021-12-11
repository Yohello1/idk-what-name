#include <SDL2/SDL.h>
#include <random>
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
    uint8_t  r,g,b,a;
    uint16_t temperature;
    uint8_t pressure;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];

struct cord_2d
{
    int x_pos;
    int y_pos;
};

void start()
{
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
}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
