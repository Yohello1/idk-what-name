#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
// #include <sycl.hpp>

// --- Configuration ---
const int BUFFER_WIDTH = 640;
const int BUFFER_HEIGHT = 360;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;
uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

// Sycl Data


// Meta Balls data
float metaBall_surface_min = 500.0;
float metaBall_surface_max = 600.0;
uint32_t metaBalls_amt = 30.0;
uint32_t metaBall_strength_max = 3000;
uint32_t metaBall_strength_min = 2000;

float* metaBall_strength_map = new float[BUFFER_WIDTH * BUFFER_HEIGHT];


typedef struct s_metaBall {
    float x_p;
    float y_p;
    float x_v;
    float y_v;

    float strength;
};

s_metaBall* metaBalls = new s_metaBall[metaBalls_amt];

void initMetaBalls()
{
    for(uint32_t i = 0; i < metaBalls_amt; i++)
    {
        metaBalls[i].x_p = rand() % BUFFER_WIDTH;
        metaBalls[i].y_p = rand() % BUFFER_HEIGHT;
        metaBalls[i].x_v = (float) ((rand() % 1000) - 500)/1000;
        metaBalls[i].y_v = (float) ((rand() % 1000) - 500)/1000;

        metaBalls[i].strength = (rand() % (metaBall_strength_max - metaBall_strength_min)) + metaBall_strength_min;
    }
}

void drawMetaBalls(uint8_t* screen_buffer, s_metaBall* balls)
{
    std::memset(metaBall_strength_map, 0.0f, sizeof(float)*BUFFER_WIDTH*BUFFER_HEIGHT);
    for(uint32_t i = 0; i < BUFFER_HEIGHT; i++) // y
    {
        for(uint32_t j = 0; j < BUFFER_WIDTH; j++) // x
        {
            for(uint32_t k = 0; k < metaBalls_amt; k++)
            {
                metaBall_strength_map[i*BUFFER_WIDTH+j] += ((float) metaBalls[k].strength)/(std::sqrt(std::pow((float) i-metaBalls[k].x_p,2.0)+std::pow((float) j-metaBalls[k].y_p,2.0)));
            }
        }
    }

    std::memset(static_rgb_buffer, 0.0f, sizeof(uint8_t)*BUFFER_WIDTH*BUFFER_HEIGHT*BYTES_PER_PIXEL);
    for(int i = 0; i < BUFFER_HEIGHT; i++)
    {
        for(int j = 0; j < BUFFER_WIDTH; j++)
        {
            if(metaBall_strength_map[i*BUFFER_WIDTH+j] >= metaBall_surface_min)
            {
                static_rgb_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL+1] = 255;
            }
        }
    }
}

void simulateMetaBalls()
{
    for(int i = 0; i < metaBalls_amt; i++)
    {
                //std::cout << i;
        metaBalls[i].x_p += metaBalls[i].x_v;
        metaBalls[i].y_p += metaBalls[i].y_v;

            metaBalls[i].x_p = std::fmax(metaBalls[i].x_p, 1.0);
            metaBalls[i].x_v *= std::signbit(metaBalls[i].x_p);

        if(metaBalls[i].y_p < 0)
        {
            metaBalls[i].x_p = std::fmax(metaBalls[i].x_p, 1.0);
            metaBalls[i].y_p = std::fmax(metaBalls[i].y_p, 1.0);
            metaBalls[i].y_v *= -1;
        }
        else if (metaBalls[i].x_p > BUFFER_WIDTH )
        {
            metaBalls[i].x_p = std::fmin(metaBalls[i].x_p, (float)BUFFER_WIDTH-1);
            metaBalls[i].y_p = std::fmin(metaBalls[i].y_p, (float)BUFFER_HEIGHT-1);
            metaBalls[i].x_v *= -1;
        }
        else if (metaBalls[i].y_p > BUFFER_HEIGHT)
        {
            metaBalls[i].x_p = std::fmax(metaBalls[i].x_p, 1.0);
            metaBalls[i].y_p = std::fmax(metaBalls[i].y_p, 1.0);
            metaBalls[i].y_v *= -1;
        }
    }
}

// --- Drawing function that fills the static array ONCE ---
void InitializeStaticBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {
            
            // Calculate the starting index for the R, G, B components of the current pixel
            int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            // --- Static Black-to-Blue Gradient Calculation ---
            
            // Calculate blue component based on X position (0 to 255)
            uint8_t blue = (uint8_t)std::min(255.0, (double)x / BUFFER_WIDTH * 255.0);

            uint8_t red = 0;   // Static Red component
            uint8_t green = 0; // Static Green component

            // Store the RGB components in the array
            // Index + 0: Red
            static_rgb_buffer[buffer_index + 0] = 0; 
            // Index + 1: Green
            static_rgb_buffer[buffer_index + 1] = 0;
            // Index + 2: Blue
            static_rgb_buffer[buffer_index + 2] = 0;
        }
    }
}

int main(int argc, char* argv[]) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    initMetaBalls();

    // 2. Create and fill the static array ONCE
    InitializeStaticBuffer();

    // 3. Create a window
    SDL_Window* window = SDL_CreateWindow(
        "SDL Static C-Style Array Buffer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BUFFER_WIDTH * SCREEN_SCALE, 
        BUFFER_HEIGHT * SCREEN_SCALE,
        SDL_WINDOW_SHOWN
    );

    // 4. Create an SDL_Surface that points to our C-style array
    // We use the array name 'static_rgb_buffer' which decays to a pointer (uint8_t*)
    SDL_Surface* bufferSurface = SDL_CreateRGBSurfaceFrom(
        static_rgb_buffer, // C-style array name acts as pointer to data
        BUFFER_WIDTH,
        BUFFER_HEIGHT,
        24, // Bits per pixel (3 bytes)
        BUFFER_WIDTH * BYTES_PER_PIXEL, // Pitch (bytes per row)
        0x00FF0000, // R-mask (standard little-endian 24-bit RGB)
        0x0000FF00, // G-mask
        0x000000FF, // B-mask
        0x00000000  // No Alpha
    );

    if (bufferSurface == nullptr) {
        std::cerr << "Could not create buffer surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 5. Get the destination (window) surface
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    // Main loop variables
    bool quit = false;
    SDL_Event e;

    // 6. Main application loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        drawMetaBalls(NULL, NULL);
        simulateMetaBalls();
        std::cout << "hi" << '\n';

        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

        SDL_UpdateWindowSurface(window);

        SDL_Delay(16);
    }

    // 8. Clean up resources
    // We only free the SDL_Surface wrapper; the C-style array memory is managed by the program.
    SDL_FreeSurface(bufferSurface); 
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
