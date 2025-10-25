#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

// --- Configuration ---
const int BUFFER_WIDTH = 600;
const int BUFFER_HEIGHT = 600;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

// This array will hold BUFFER_WIDTH * BUFFER_HEIGHT * 3 bytes (R, G, B)
uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

// Agents Data
typedef struct agentData
{
    float x_p;
    float y_p;
    float x_v;
    float y_v;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

uint32_t agentsAmt = 3000;
agentData* agentsData = new agentData[agentsAmt]; 

void drawAgents()
{
    for(int i = 0; i < agentsAmt; i++)
    {
        float safe_x = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, agentsData[i].x_p));
        float safe_y = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, agentsData[i].y_p));
        uint32_t buffer_index = ((uint32_t)safe_y * BUFFER_WIDTH + (uint32_t)safe_x) * BYTES_PER_PIXEL;

        static_rgb_buffer[buffer_index] = agentsData[i].r;
        static_rgb_buffer[buffer_index+1] = agentsData[i].g;
        static_rgb_buffer[buffer_index+2] = agentsData[i].b;
    }
}

void initAgents()
{
    for(int i = 0; i < agentsAmt; i++)
    {
        agentsData[i].x_p = BUFFER_WIDTH/2;
        agentsData[i].y_p = BUFFER_HEIGHT/2;
        agentsData[i].x_v = ((rand() % 100) - 50.0)/100.0;
        agentsData[i].y_v = ((rand() % 100) - 50.0)/100.0;
        agentsData[i].r = rand() % 256;
        agentsData[i].g = rand() % 256;
        agentsData[i].b = rand() % 256;

        std::cout << ((rand() % 100) - 50.0)/100.0 << 'a'; 
    }
}

void simulateAgents()
{
    for(int i = 0; i < agentsAmt; i++)
    {
        //std::cout << i;
        agentsData[i].x_p += agentsData[i].x_v;
        agentsData[i].y_p += agentsData[i].y_v;
   
        if(agentsData[i].x_p < 0)
        {
            agentsData[i].x_p = std::fmax(agentsData[i].x_p, 1.0);
            agentsData[i].y_p = std::fmax(agentsData[i].y_p, 1.0);
            agentsData[i].x_v *= -1;
        }

        else if(agentsData[i].y_p < 0)
        {
            agentsData[i].x_p = std::fmax(agentsData[i].x_p, 1.0);
            agentsData[i].y_p = std::fmax(agentsData[i].y_p, 1.0);
            agentsData[i].y_v *= -1;
        }
        else if (agentsData[i].x_p > BUFFER_WIDTH )
        {
            agentsData[i].x_p = std::fmin(agentsData[i].x_p, (float)BUFFER_WIDTH-1);
            agentsData[i].y_p = std::fmin(agentsData[i].y_p, (float)BUFFER_HEIGHT-1     );
            agentsData[i].x_v *= -1;
        }
        else if (agentsData[i].y_p > BUFFER_HEIGHT)
        {
            agentsData[i].x_p = std::fmax(agentsData[i].x_p, 1.0);
            agentsData[i].y_p = std::fmax(agentsData[i].y_p, 1.0);
            agentsData[i].y_v *= -1;
        }
    

       // std::cout << ' '  <<agentsData[i].x_p << ' ' << agentsData[i].x_v << '\n';
    }

    for(int i = 0; i < (BUFFER_HEIGHT*BUFFER_WIDTH*BYTES_PER_PIXEL); i++)
    {
        if(static_rgb_buffer[i] > 0)
            static_rgb_buffer[i]--;
    }

}

// --- Drawing function that fills the static array ONCE ---
void InitializeStaticBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {
            
            // Calculate the starting index for the R, G, B components of the current pixel
            int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
            static_rgb_buffer[buffer_index + 0] = 10; 
            static_rgb_buffer[buffer_index + 1] = 10;
            static_rgb_buffer[buffer_index + 2] = 10;
        }
    }
}

int main(int argc, char* argv[]) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    srand(time(NULL));
    initAgents();

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

        drawAgents();


        // --- COPY (BLIT) STATIC BUFFER TO SCREEN ---
        // Copies the fixed gradient image onto the window surface.
        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

        simulateAgents();

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
