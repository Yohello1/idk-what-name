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
#include "simulate.hpp"
#include "poly6.hpp"
#include "spiky_k.hpp"
#include "viscosity_k.hpp"

void copyFloaters()
{
    std::memcpy(floatersB, floatersA, sizeof(floater)*FLOATER_AMT);
}

void swapFloaters()
{
    void* temp = (void*) floatersA;
    floatersA = floatersB;
    floatersB = (floater*) temp;
}

void simulateFloaters()
{
    JD::simulate::computeDensity<JD::Poly6_k::smoothing>(offsets, cells_ctr, particles_loc, floatersA, PARTICLE_SIZE);
    JD::simulate::computePressureForce<JD::Spiky_k::gradient>(offsets, cells_ctr, particles_loc, floatersA, PARTICLE_SIZE);
    JD::simulate::computeViscosity<JD::Viscosity_k::laplacian>(offsets, cells_ctr, particles_loc, floatersA, PARTICLE_SIZE);
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
        "draw2",
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
       
        simulateFloaters();
        // code heheh
        // swapFloaters();
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

