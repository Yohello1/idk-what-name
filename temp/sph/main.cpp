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



void initfloatersA()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
        floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING; 

        floatersA[i].v_x = ((rand() % 50) - 25)/10;
        floatersA[i].v_y = ((rand() % 50) - 25)/10;

        floatersA[i].r = (rand() % 127)+128;
        floatersA[i].b = (rand() % 127)+128;

        floatersA[i].density = PARTICLE_REFERENCE_DENSITY;
    
        // std::cout << floatersA[i].density << ' ';
    }
    // std::cout << '\n';
}

void initGrid()
{

    std::vector<std::pair<int, int>> derivative = calculateRegionsOffsets();

    std::vector<int> relative_offsets {};
    for(int i = 0; i < REGIONS_AMT; i++) {


        relative_offsets.push_back(derivative[i].first + (derivative[i].second * BUFFER_LINE));
    }

    std::sort(relative_offsets.begin(), relative_offsets.end());

    for(int i = 0; i < POINTS_AMT; i++)
    {
        int x = (i%POINTS_WIDTH);
        int y = (i/POINTS_WIDTH);

        points[i].x = x;
        points[i].y = y;

        points[i].i_x = x*DISTANCE_BETWEEN_POINTS + BUFFER_PADDING;
        points[i].i_y = y*DISTANCE_BETWEEN_POINTS + BUFFER_PADDING;

        points[i].id = i;

        points[i].strength = rand() % 256;

        int cell_x = points[i].i_x / DISTANCE_BETWEEN_POINTS;
        int cell_y = points[i].i_y / DISTANCE_BETWEEN_POINTS;

        int base_cell_id = cell_x + BUFFER_LINE*cell_y;

        for(int j = 0; j < REGIONS_AMT; j++)
        {
            points[i].regions[j] = base_cell_id + relative_offsets[j];
        }
    }

}


void drawfloatersA()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int idx = floatersA[i].x*BYTES_PER_PIXEL + floatersA[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
        static_rgb_buffer[idx]   = floatersA[i].r;
        static_rgb_buffer[idx+1] = floatersA[i].b;
    }
}

void simulatefloatersA()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floatersA[i].x += floatersA[i].v_x;
        floatersA[i].y += floatersA[i].v_y;


        if (floatersA[i].x < (float) BUFFER_PADDING)
        {
            floatersA[i].v_x *= -1.0f;
            floatersA[i].x = BUFFER_PADDING;
        }


        else if (floatersA[i].x > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floatersA[i].v_x *= -1.0f;
            floatersA[i].x = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }


        if (floatersA[i].y < (float) BUFFER_PADDING)
        {
            floatersA[i].v_y *= -1.0f;
            floatersA[i].y = BUFFER_PADDING;
        }

        else if (floatersA[i].y > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floatersA[i].v_y *= -1.0f;
            floatersA[i].y = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }
    }
}

void computeStrengths()
{
    floater* __restrict p_floatersA = floatersA;
    int* __restrict p_indices = particles_loc;

    for(int i = 0; i < POINTS_AMT; i++)
    {
        float strength = 0.0f;

        int x = points[i].i_x;
        int y = points[i].i_y;


        for(int j = 0; j < REGIONS_AMT; j++)
        {
            int idx_r = points[i].regions[j];
            int idx_o = offsets[idx_r];


            for(int k = 0; k < cells_ctr[idx_r]; k++)
            {
                int floater_idx = p_indices[idx_o + k];

                float dx = p_floatersA[floater_idx].x - x;
                float dy = p_floatersA[floater_idx].y - y;
                float dist_sq = dx*dx + dy*dy;

                // if(dist_sq < 0.001f) dist_sq = 0.001f;
                dist_sq = (dist_sq < 0.001f) ? 0.001 : dist_sq; 

                float val = (p_floatersA[floater_idx].density)* (1.0f / dist_sq);

                // std::cout << p_floatersA[floater_idx].density << ' ';
                //float val = (floatersA[particles_loc[idx_o+k]].r+floatersA[particles_loc[idx_o+k]].b)/((floatersA[particles_loc[idx_o+k]].x-x)*(floatersA[particles_loc[idx_o+k]].x-x)+(floatersA[particles_loc[idx_o+k]].y-y)*(floatersA[particles_loc[idx_o+k]].y-y));

                strength += val;
            }

        }

        points[i].strength = (uint16_t)strength;
    }
}

void drawConnections()
{
    const int SQR_PER_ROW = POINTS_WIDTH - 1;
    const int SQR_PER_COL = POINTS_HEIGHT - 1;

    static const int8_t LUT[16][4] = {
        {-1,-1,-1,-1}, {0,3,-1,-1}, {0,1,-1,-1}, {3,1,-1,-1},
        {1,2,-1,-1},   {0,1,2,3},   {0,2,-1,-1}, {3,2,-1,-1},
        {3,2,-1,-1},   {0,2,-1,-1}, {0,3,1,2},   {1,2,-1,-1},
        {3,1,-1,-1},   {0,1,-1,-1}, {0,3,-1,-1}, {-1,-1,-1,-1}
    };

    const int R = 255, G = 255, B = 255;

    for(int i = 0; i < SQR_PER_ROW; i++)
    {
        int row_top = i * POINTS_WIDTH;
        int row_bot = (i + 1) * POINTS_WIDTH;

        for(int j = 0; j < SQR_PER_COL; j++)
        {
            int config_index = (points[row_top + j].strength     >= THRESHOLD)      |
                               ((points[row_top + j + 1].strength >= THRESHOLD) << 1) |
                               ((points[row_bot + j + 1].strength >= THRESHOLD) << 2) |
                               ((points[row_bot + j].strength     >= THRESHOLD) << 3);

            if (config_index == 0 || config_index == 15) continue;

            std::pair<int, int> pts[4];
            pts[0] = get_mid_point(points[row_top + j],     points[row_top + j + 1]); // pA
            pts[1] = get_mid_point(points[row_top + j + 1], points[row_bot + j + 1]); // pB
            pts[2] = get_mid_point(points[row_bot + j + 1], points[row_bot + j]);     // pC
            pts[3] = get_mid_point(points[row_bot + j],     points[row_top + j]);     // pD

            const int8_t* edges = LUT[config_index];

            draw_line_std_pair(static_rgb_buffer, pts[edges[0]], pts[edges[1]], R, G, B);

            if (edges[2] != -1) {
                draw_line_std_pair(static_rgb_buffer, pts[edges[2]], pts[edges[3]], R, G, B);
            }
        }
    }
}

void drawGrid()
{
    for(int i = 0; i < POINTS_AMT; i++)
    {
        int idx = points[i].i_x*BYTES_PER_PIXEL + points[i].i_y*BUFFER_WIDTH*BYTES_PER_PIXEL;

        uint16_t strength = points[i].strength;

        if(!(strength >= THRESHOLD))
            strength = 0;

        static_rgb_buffer[idx] = 255;
        static_rgb_buffer[idx+1] = 255;
        static_rgb_buffer[idx+2] = 255;

    }
}

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
    initfloatersA();

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
        simulatefloatersA();
        drawfloatersA();
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

