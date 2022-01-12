#include <SDL2/SDL.h>
#include <random>
#define LOGICAL_WINDOW_WIDTH 1024
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

struct cord_2d
{
    int x_pos;
    int y_pos;
};

typedef std::chrono::high_resolution_clock Clock;
