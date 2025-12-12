#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

// --- Configuration ---
const int BUFFER_WIDTH = 720;
const int BUFFER_HEIGHT = 720;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

const int DISTANCE_BETWEEN_POINTS = 8;
constexpr int POINTS_AMT = (BUFFER_WIDTH/DISTANCE_BETWEEN_POINTS)*(BUFFER_HEIGHT/DISTANCE_BETWEEN_POINTS);
constexpr int POINTS_WIDTH = (BUFFER_WIDTH/DISTANCE_BETWEEN_POINTS);
constexpr int POINTS_HEIGHT = (BUFFER_HEIGHT/DISTANCE_BETWEEN_POINTS);

const uint16_t  THRESHOLD = 180;

// The C-STYLE STATIC ARRAY for our pixel data
// This array will hold BUFFER_WIDTH * BUFFER_HEIGHT * 3 bytes (R, G, B)
uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];


struct point
{
    uint16_t x;
    uint16_t y;

    uint16_t i_x;
    uint16_t i_y;

    uint16_t id;
    uint16_t strength;
    
};

struct point* points = new struct point[POINTS_AMT];

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

std::pair<int, int> get_mid_point(const point& p0, const point& p1)
{
    return {(p0.i_x*p0.strength+p1.i_x*p1.strength)/(p1.strength+p0.strength) , (p0.i_y*p0.strength+p1.i_y*p1.strength)/(p1.strength+p0.strength) };
}

void draw_line_std_pair(uint8_t* buffer, 
                        std::pair<int, int> p0, 
                        std::pair<int, int> p1, 
                        uint8_t r, uint8_t g, uint8_t b) {
    
    int x0 = p0.first;
    int y0 = p0.second;
    int x1 = p1.first;
    int y1 = p1.second;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2;
    int e2; 

    int x = x0;
    int y = y0;

    while (true) {
        if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
            int index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            static_rgb_buffer[index + 0] = 255;
            static_rgb_buffer[index + 1] = 255;
            static_rgb_buffer[index + 2] = 255;
        }

        if (x == x1 && y == y1) break;

        e2 = err;
        
        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }

        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
}

void initGrid()
{
    

    for(int i = 0; i < POINTS_AMT; i++)
    {
        int x = (i%POINTS_WIDTH);
        int y = (i/POINTS_WIDTH);
        
        std::cout << x << ',' << y;

        points[i].x = x; 
        points[i].y = y;

        points[i].i_x = x*DISTANCE_BETWEEN_POINTS; 
        points[i].i_y = y*DISTANCE_BETWEEN_POINTS; 

        points[i].id = i;
        
        points[i].strength = rand() % 256;

    }
}




void drawConnections()
{
    int offset = DISTANCE_BETWEEN_POINTS/2;    

    const int SQR_PER_ROW = POINTS_WIDTH - 1;
    const int SQR_PER_COL = POINTS_WIDTH - 1;


    for(int i = 0; i < SQR_PER_ROW; i++)
    {
        for(int j = 0; j < SQR_PER_COL; j++)
        {
            // 0 - 1
            // - X -
            // 2 - 3

            // 0 = sqr_per_row*row         + col
            // 1 = sqr_per_row*row     + 1 + col
            // 2 = (sqr_per_row+1)*row     + col
            // 3 = (sqr_per_row+1)*row + 1 + col
            
            int idx_0 = POINTS_WIDTH*i + j; 
            int idx_1 = POINTS_WIDTH*i + j + 1; 
            int idx_2 = POINTS_WIDTH*(1+i) + j; 
            int idx_3 = POINTS_WIDTH*(1+i) + j + 1;

            bool idx_0_b = (points[idx_0].strength >= THRESHOLD);
            bool idx_1_b = (points[idx_1].strength >= THRESHOLD);
            bool idx_2_b = (points[idx_2].strength >= THRESHOLD);
            bool idx_3_b = (points[idx_3].strength >= THRESHOLD);

            int config_index = 0;
            if (idx_0_b) config_index |= 1;
            if (idx_1_b) config_index |= 2;
            if (idx_3_b) config_index |= 4;
            if (idx_2_b) config_index |= 8;

            std::pair<int, int> pA = get_mid_point(points[idx_0], points[idx_1]);
            std::pair<int, int> pB = get_mid_point(points[idx_1], points[idx_3]);
            std::pair<int, int> pC = get_mid_point(points[idx_3], points[idx_2]);
            std::pair<int, int> pD = get_mid_point(points[idx_2], points[idx_0]);

            int R = 255, G = 255, B = 255;

            std::cout << i << ' ' << j << ' ' << idx_0 << ' ' << idx_1 << ' ' << idx_2 << ' ' << idx_3 << '\n';

            switch (config_index)
            {
                case 0:
                case 15:
                    break;

                case 1: draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B); break; // P0
                case 2: draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B); break; // P1
                case 4: draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B); break; // P3
                case 8: draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B); break; // P2

                case 14: draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B); break;
                case 13: draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B); break;
                case 11: draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B); break;
                case 7: draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B); break;

                case 3: draw_line_std_pair(static_rgb_buffer, pD, pB, R, G, B); break;
                case 12: draw_line_std_pair(static_rgb_buffer, pD, pB, R, G, B); break;

                case 6: draw_line_std_pair(static_rgb_buffer, pA, pC, R, G, B); break;
                case 9: draw_line_std_pair(static_rgb_buffer, pA, pC, R, G, B); break;

                case 5:
                    draw_line_std_pair(static_rgb_buffer, pA, pB, R, G, B);
                    draw_line_std_pair(static_rgb_buffer, pC, pD, R, G, B);
                    break;
                case 10:
                    draw_line_std_pair(static_rgb_buffer, pA, pD, R, G, B);
                    draw_line_std_pair(static_rgb_buffer, pB, pC, R, G, B);
                    break;
            }

        }
    }

    std::cout << "dimensions: " << SQR_PER_ROW << ' ' << SQR_PER_COL << '\n';

}

void drawGrid()
{
    std::cout << "hI" << std::endl;
    for(int i = 0; i < POINTS_AMT; i++)
    {
        int idx = points[i].i_x*BYTES_PER_PIXEL + points[i].i_y*BUFFER_WIDTH*BYTES_PER_PIXEL;

        uint16_t strength = points[i].strength;

        if(!(strength >= THRESHOLD))
            strength = 0;

        static_rgb_buffer[idx] = strength;
        static_rgb_buffer[idx+1] = strength;
        static_rgb_buffer[idx+2] = strength;

    }

    drawConnections();
}

int main(int argc, char* argv[]) {
    

    initGrid();
    
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

    // 6. Main application loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        drawGrid();

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
