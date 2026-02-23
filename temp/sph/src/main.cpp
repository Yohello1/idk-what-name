#include "settings.hpp"
#include "struct.hpp"
#include "floaters.hpp"
#include "graphics.hpp"
#include "spatial.hpp"
#include "simulate.hpp"
#include "poly6.hpp"
#include "viscosity_k.hpp"
#include "gravity.hpp"
#include "math.hpp"
#include "spiky_k.hpp"
#include <SDL2/SDL.h>
#include <omp.h>

#include <cstring>
#include <iostream>
#include <iomanip>

void copyFloaters() {
    // std::memcpy(JD::floaters::floatersB, JD::floaters::floatersA, sizeof(floater) * FLOATER_AMT);
}

void simulateFloaters()
{
    JD::simulate::computeDensity<JD::Poly6_k::smoothing>(
        JD::graphics::offsets,
        JD::graphics::cells_ctr,
        JD::graphics::particles_loc,
        JD::floaters::BLOCK_NEIGHBOR_COUNT,
        JD::floaters::blocks,
        JD::floaters::floatersA,
        PARTICLE_SIZE);
    JD::simulate::computePressureForce<JD::Spiky_k::gradient>(
        JD::graphics::offsets,
        JD::graphics::cells_ctr,
        JD::graphics::particles_loc,
        JD::floaters::floatersA,
        PARTICLE_SIZE);
    JD::simulate::computeViscosity<JD::Viscosity_k::laplacian>(
        JD::graphics::offsets,
        JD::graphics::cells_ctr,
        JD::graphics::particles_loc,
        JD::floaters::floatersA,
        PARTICLE_SIZE);
    JD::simulate::applyYAccelerationToAllParticles<JD::gravity::gravityAcceleration>(
        JD::floaters::floatersA);
    JD::simulate::integrate(
        JD::graphics::offsets,
        JD::graphics::cells_ctr,
        JD::graphics::particles_loc,
        JD::floaters::floatersA);
}


int main() {
    JD::graphics::initGrid();
    JD::floaters::init();
    srand(100);

    std::cout << "BUFFER_LINE: " << BUFFER_LINE << std::endl;
    std::cout << "DISTANCE_BETWEEN_POINTS: " << DISTANCE_BETWEEN_POINTS << std::endl;
    std::cout << "SIZE_MULTIPLIER: " << SIZE_MULTIPLIER << std::endl;
    std::cout << "PADDING: " << PADDING << std::endl;
    std::cout << "BUFFER_WIDTH:  " << BUFFER_WIDTH << std::endl;
    std::cout << "BUFFER_HEIGHT: " << BUFFER_HEIGHT << std::endl;

    std::cout << std::fixed << std::setprecision(2);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    JD::graphics::InitializeStaticBuffer();

    SDL_Window* window = SDL_CreateWindow(
        "Viewport Render",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, 
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    SDL_Surface* bufferSurface = SDL_CreateRGBSurfaceFrom(
        ::JD::graphics::static_rgb_buffer, 
        BUFFER_WIDTH,
        BUFFER_HEIGHT,
        24, 
        BUFFER_WIDTH * BYTES_PER_PIXEL,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000
    );

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    SDL_Rect viewRect;
    viewRect.x = 0;
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
            
             if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    viewRect.y -= 10; std::cout << "hi" << std::endl; break;
                    case SDLK_DOWN:  viewRect.y += 10; break;
                    case SDLK_LEFT:  viewRect.x -= 10; break;
                    case SDLK_RIGHT: viewRect.x += 10; break;
                }
            }
        }

        if (viewRect.x < 0) viewRect.x = 0;
        if (viewRect.y < 0) viewRect.y = 0;
        if (viewRect.x + viewRect.w > BUFFER_WIDTH) viewRect.x = BUFFER_WIDTH - viewRect.w;
        if (viewRect.y + viewRect.h > BUFFER_HEIGHT) viewRect.y = BUFFER_HEIGHT - viewRect.h;

        memset(JD::graphics::static_rgb_buffer, 0, (size_t)BUFFER_HEIGHT * BUFFER_WIDTH * BYTES_PER_PIXEL);
        
        JD::floaters::drawFloaters();
        JD::spatial::offsetsCreation();
        JD::spatial::computeIndicies();
        JD::graphics::computeStrengths();
        // copyFloaters(); // not needed in new version!
        JD::graphics::drawConnections();

        SDL_BlitSurface(bufferSurface, &viewRect, screenSurface, nullptr);
        SDL_UpdateWindowSurface(window);

       
        simulateFloaters();

        
        static int frame_num = 0;
        
         if (frame_num % 10 == 0) {  // print every 10 frames to avoid spam
            SimDiag diag = collectDiagnostics(JD::floaters::floatersA, frame_num);
            printDiagnostics(diag, frame_num);
        }
        
        frame_num++;

        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        std::cout << "Frame " << (frame_num-1)
                  << " Time: " << std::fixed << std::setprecision(4) << time_taken << "s" << std::endl;
    }

    SDL_FreeSurface(bufferSurface);
    SDL_DestroyWindow(window);
    SDL_Quit();

     return 0;
}
