#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <algorithm>
#include <vector>

// --- Configuration ---
const int BUFFER_WIDTH = 720;
const int BUFFER_HEIGHT = 720;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

std::vector<std::pair<int, int>> points;

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
            uint8_t blue = 0; 

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

void drawPoints()
{
    for(size_t i = 0; i < points.size(); i++)
    {
        static_rgb_buffer[BYTES_PER_PIXEL*(BUFFER_WIDTH*points[i].second + points[i].first)+1] = 255;
    }
}

void drawLines(std::pair<int, int> a, std::pair<int, int> b)
{
    float dx = b.first - a.first;
    float dy = b.second - a.second;

    int steps = std::max(std::abs(dx), std::abs(dy));

    float xIncr = dx / (float)steps;
    float yIncr = dy / (float)steps;

    float x = a.first;
    float y = a.second;

    for(int i = 0; i <= steps; i++)
    {
        int p_x = std::round(x);
        int p_y = std::round(y);

        if(p_x >= 0 && p_x < BUFFER_WIDTH && p_y >= 0 && p_y < BUFFER_HEIGHT)
        {
            static_rgb_buffer[BYTES_PER_PIXEL*(BUFFER_WIDTH*p_y + p_x) + 2] = 255;
        }
        
        x += xIncr;
        y += yIncr;
    }

}

void drawCurves()
{
    for(int i = 0; i < (points.size() - 2); i++)
    {

        float dx_1 = points[i+1].first -  points[i].first;
        float dy_1 = points[i+1].second - points[i].second;

    
        float dx_2 = points[i+2].first  - points[i+1].first;
        float dy_2 = points[i+2].second - points[i+1].second;

        int steps = std::max({std::abs(dx_1), std::abs(dy_1), std::abs(dx_2), std::abs(dy_2)});
        
        float xIncr_1 = dx_1 / (float)steps;
        float yIncr_1 = dy_1 / (float)steps;
        
        float xIncr_2 = dx_2 / (float)steps;
        float yIncr_2 = dy_2 / (float)steps;


        float x_1 = points[i].first;
        float y_1 = points[i].second;

        float x_2 = points[i+1].first;
        float y_2 = points[i+1].second;

        for(int j = 0; j <= steps; j++)
        {
            float t = (float) j / (float) steps;

            int p_x = std::round((x_1 + t*(x_2-x_1))/2);
            int p_y = std::round((y_1 + t*(y_2-y_1))/2);



            if(p_x >= 0 && p_x < BUFFER_WIDTH && p_y >= 0 && p_y < BUFFER_HEIGHT)
            {
                static_rgb_buffer[BYTES_PER_PIXEL*(BUFFER_WIDTH*p_y + p_x) + 2] = 255;
            }
            
            x_1 += xIncr_1;
            y_1 += yIncr_1;
        
            x_2 += xIncr_2;
            y_2 += yIncr_2;
        }
    }
}


int main(int argc, char* argv[]) {
    // points
    if(argc % 2 == 0) { std::cout << "weird incorr num of args" << std::endl; return 0; } 
    for(int i = 1; i < argc; i+= 2)
    {
        points.push_back(std::make_pair<int, int>(std::stoi(argv[i]), std::stoi(argv[i+1]))); 
    }


    for(size_t i = 0; i < points.size(); i++)
    {
        std::cout << points[i].first << ", " << points[i].second << std::endl;
    }

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

         drawLines(points[0], points[1]);
    drawCurves();
    // 6. Main application loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        drawPoints();
        
        for(int i = 0; i < (points.size() - 1); i ++)
        {
            drawLines(points[i], points[i+1]);
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
