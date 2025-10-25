#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

// --- Configuration ---
const int BUFFER_WIDTH = 320;
const int BUFFER_HEIGHT = 240;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

// The C-STYLE STATIC ARRAY for our pixel data
// This array will hold BUFFER_WIDTH * BUFFER_HEIGHT * 3 bytes (R, G, B)
uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

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
            static_rgb_buffer[buffer_index + 0] = red; 
            // Index + 1: Green
            static_rgb_buffer[buffer_index + 1] = green;
            // Index + 2: Blue
            static_rgb_buffer[buffer_index + 2] = blue;
        }
    }
}

int main(int argc, char* argv[]) {
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

    // 6. Main application loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // --- COPY (BLIT) STATIC BUFFER TO SCREEN ---
        // Copies the fixed gradient image onto the window surface.
        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

        // 7. Update the window to display the fixed changes
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
