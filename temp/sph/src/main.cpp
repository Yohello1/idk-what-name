#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>
#include <omp.h>

// Assuming these headers define BUFFER_WIDTH, BUFFER_HEIGHT, etc.
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
#include "gravity.hpp"

void copyFloaters() {
    std::memcpy(JD::graphics::floatersB, JD::graphics::floatersA, sizeof(floater) * FLOATER_AMT);
}

int main(int argc, char* argv[]) {
    JD::graphics::initGrid();
    JD::floaters::initFloaters();
    srand(100);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    JD::graphics::InitializeStaticBuffer();

    // 1. Create the window with the FIXED viewport size
    SDL_Window* window = SDL_CreateWindow(
        "Viewport Render",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, 
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    // 2. This surface represents your "Infinite/Large Canvas"
    SDL_Surface* bufferSurface = SDL_CreateRGBSurfaceFrom(
        ::JD::graphics::static_rgb_buffer, 
        BUFFER_WIDTH,   // The actual large data width
        BUFFER_HEIGHT,  // The actual large data height
        24, 
        BUFFER_WIDTH * BYTES_PER_PIXEL,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000
    );

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    // 3. Define the Viewport (The "Camera")
    SDL_Rect viewRect;
    viewRect.x = 0;      // Start looking at top-left
    viewRect.y = 0;
    viewRect.w = WINDOW_WIDTH;
    viewRect.h = WINDOW_HEIGHT;

    bool quit = false;
    SDL_Event e;
    clock_t start, end;

    while (!quit) {
        start = clock();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            
            // Example: Basic Camera Control with Arrow Keys
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    viewRect.y -= 10; break;
                    case SDLK_DOWN:  viewRect.y += 10; break;
                    case SDLK_LEFT:  viewRect.x -= 10; break;
                    case SDLK_RIGHT: viewRect.x += 10; break;
                }
            }
        }

        // --- CAMERA BOUNDARY CHECKING ---
        if (viewRect.x < 0) viewRect.x = 0;
        if (viewRect.y < 0) viewRect.y = 0;
        if (viewRect.x + viewRect.w > BUFFER_WIDTH) viewRect.x = BUFFER_WIDTH - viewRect.w;
        if (viewRect.y + viewRect.h > BUFFER_HEIGHT) viewRect.y = BUFFER_HEIGHT - viewRect.h;

        // Clear the large back-buffer
        memset(JD::graphics::static_rgb_buffer, 0, (size_t)BUFFER_HEIGHT * BUFFER_WIDTH * BYTES_PER_PIXEL);

        // Simulation and Drawing calls (drawing to the large buffer)
        JD::floaters::drawFloaters();
        copyFloaters();
        JD::graphics::drawConnections();

        // --- THE VIEWPORT BLIT ---
        // Source: bufferSurface (the whole world), but only the part inside viewRect
        // Destination: screenSurface (the window), starting at 0,0
        SDL_BlitSurface(bufferSurface, &viewRect, screenSurface, nullptr);

        SDL_UpdateWindowSurface(window);

        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "Frame Time: " << std::fixed << std::setprecision(4) << time_taken << "s" << std::endl;

        // Custom simulation steps
        offsetsCreation();
        computeIndicies();
        SDL_Delay(25);
    }

    SDL_FreeSurface(bufferSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
