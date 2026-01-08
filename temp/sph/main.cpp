#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>
#include <omp.h>


#include "settings.hpp"
#include "struct.hpp"
#include "math.hpp"
#include "graphics.hpp"
#include "spatial.hpp"
#include "floaters.hpp"

void computePressure_p6()
{
    float constant = 315.0/(64.0 * 3.141592654 * std::pow(PARTICLE_SIZE, 9.0f));

    for(int i = 0; i < FLOATER_AMT; i++)
    {
        std::pair<float, float> summed;
        summed.first = 0.0f;
        summed.second = 0.0f;
        for(int j = 0; j < FLOATER_AMT; j++)
        {
            if(j == i) continue;

            float distance = std::sqrt(std::pow(floatersA[i].x - floatersA[j].x, 2.0f) + std::pow(floatersA[i].y - floatersA[j].y, 2.0f));
            
            if(distance > PARTICLE_SIZE) continue;    

            float temp = -6.0f * std::pow(std::pow(PARTICLE_SIZE, 2.0f) -  std::pow(distance, 2.0f), 2.0f);

            float dx = floatersA[i].x - floatersA[j].x;
            float dy = floatersA[i].y - floatersA[j].y;
     
            dx *= temp;
            dy *= temp;

            summed.first += dx;
            summed.second += dy;
        }
        
        floatersB[i].p_x = summed.first;
        floatersB[i].p_y = summed.second;
    }
}

void computeViscosity()
{
    float constant = 315.0/(64.0 * 3.141592654 * std::pow(PARTICLE_SIZE, 9.0f));

    for(int i = 0; i < FLOATER_AMT; i++)
    {
        for(int j = 0; j < FLOATER_AMT; j++)
        {
            if(j == i) continue;

            float distance = std::sqrt(std::pow(floatersA[i].x - floatersA[j].x, 2.0f) + std::pow(floatersA[i].y - floatersA[j].y, 2.0f));
            
            if(distance > PARTICLE_SIZE) continue;    

            float temp = -6.0f * std::pow(std::pow(PARTICLE_SIZE, 2.0f) -  std::pow(distance, 2.0f), 2.0f);

            float dx = floatersA[i].x - floatersA[j].x;
            float dy = floatersA[i].y - floatersA[j].y;

            dx *= temp;
            dy *= temp;

//            summed.first += dx;
//            summed.second += dy;
        }
    }
}


void copyFloaters()
{
    std::memcpy(floatersB, floatersA, sizeof(floater)*FLOATER_AMT);
}

// kernel value: density
// first derivitive: force
// second derivative: viscosity
//
// Density is just a value
// Force is accel part 1
// viscoscity is used to smooth similar to other stuff

void computeDensity_p6()
{
    // 2D Constant btw
    float density_multiplier = 4.0;
    density_multiplier /= 3.141592654*std::pow(PARTICLE_SIZE, 8.0f); 
    
    // I have 0 clue how to implement this
    // Ok so it returns 

    // Density computation 

    for(int i = 0; i < FLOATER_AMT; i++)
    {
        float density = 0.0;
        for(int k = 0; k < FLOATER_AMT; k++)
        {
            if(k == i) continue;     
            
            float r = fdistEuclid({floatersA[i].x, floatersA[i].y}, {floatersA[k].x, floatersA[k].y});
            
            if(r < PARTICLE_SIZE) continue;

            float cubed = (float) std::pow((std::pow(r,2.0f), std::pow(PARTICLE_SIZE, 2.0f)), 3.0f);

            density += cubed;
        }
        density *= density_multiplier;
   
        // std::cout << density << ' ' ;

        floatersB[i].density = density;
    }
    // for(int i = 0; i < 
}

void swapFloaters()
{
    void* temp = (void*) floatersA;
    floatersA = floatersB;
    floatersB = (floater*) temp;
}


int main(int argc, char* argv[]) {

    initGrid();
    initFloaters();

    srand(100);


    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

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

    clock_t start, end;

    // 6. Main application loop
    while (!quit) {

        start = clock();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        memset(static_rgb_buffer, 0, sizeof(uint8_t)*BUFFER_HEIGHT*BUFFER_WIDTH*BYTES_PER_PIXEL);
        simulateFloaters();
        drawFloaters();
        copyFloaters();
        computeDensity_p6();
        computePressure_p6();
        swapFloaters();
        computeStrengths();
        drawConnections();
        // drawGrid();

        // --- COPY (BLIT) STATIC BUFFER TO SCREEN ---
        // Copies the fixed gradient image onto the window surface.
        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

        // 7. Update the window to display the fixed changes
        SDL_UpdateWindowSurface(window);


        end = clock();

        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);


        std::cout << "Time taken by program is : " << std::fixed
        << time_taken << std::setprecision(8) << std::endl;



        offsetsCreation();
        computeIndicies();
        SDL_Delay(25);

    }

    // 8. Clean up resources
    // We only free the SDL_Surface wrapper; the C-style array memory is managed by the program.
    SDL_FreeSurface(bufferSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

