#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>


// --- Configuration ---
const int DISTANCE_BETWEEN_POINTS = 8;

const int SIZE_MULTIPLIER = 96;
const int PADDING = 3;

const int BUFFER_WIDTH = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS+(DISTANCE_BETWEEN_POINTS*PADDING*2);
const int BUFFER_HEIGHT = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS+(DISTANCE_BETWEEN_POINTS*PADDING*2);

const int BUFFER_PADDING = PADDING*DISTANCE_BETWEEN_POINTS;
const int BUFFER_UNPADDED = PADDING*DISTANCE_BETWEEN_POINTS + SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;
const int BUFFER_LINE = PADDING*2 + SIZE_MULTIPLIER;
const int BUFFER_WORKING = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;

const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

constexpr int POINTS_WIDTH = SIZE_MULTIPLIER+1;
constexpr int POINTS_HEIGHT = SIZE_MULTIPLIER+1;
constexpr int POINTS_AMT = (POINTS_HEIGHT)*(POINTS_WIDTH);

const int FLOATER_AMT = 10000;
const int FLOATER_SPEED = 3;

const uint16_t THRESHOLD = 180;

// spatial compute stuff
constexpr int CELL_SIZE = DISTANCE_BETWEEN_POINTS*DISTANCE_BETWEEN_POINTS;
int offsets[BUFFER_LINE * BUFFER_LINE];
int cells_ctr[BUFFER_LINE * BUFFER_LINE];
int particles_loc[FLOATER_AMT];

const int REGIONS_AMT = 13; //this changes like nothing btw

uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

struct point
{
    uint16_t x;
    uint16_t y;

    uint16_t i_x;
    uint16_t i_y;

    uint16_t id;
    uint16_t strength;

    int regions[REGIONS_AMT];

};

struct floater {
    float x;
    float y;
    
    float v_x; 
    float v_y;
    
    uint8_t r;
    uint8_t b;
};

struct point*   points = new struct point[POINTS_AMT];
struct floater* floaters = new struct floater[FLOATER_AMT];

int signBit(int a) { return a/abs(a); };
float fsignBit(float a) { return a/abs(a); };

std::pair<int, int> get_mid_point(const point& p0, const point& p1)
{
    return {(p0.i_x + p1.i_x) / 2, (p0.i_y + p1.i_y) / 2};
}

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

void offsetsCreation()
{
    memset(cells_ctr, 0, sizeof(int)*(BUFFER_LINE*BUFFER_LINE));

    /*
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int x = (floaters[i].x-BUFFER_PADDING)/DISTANCE_BETWEEN_POINTS;
        int y = (floaters[i].y-BUFFER_PADDING)/DISTANCE_BETWEEN_POINTS;

        int idx = x + y*POINTS_WIDTH;

        cells_ctr[idx] += 1; 
    }

    for(int i = 0, j = 0; i < POINTS_AMT; i++)
    {
        offsets[i] = j;
        j += cells_ctr[i];
    }
    */

    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int gx = (floaters[i].x - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;
        int gy = (floaters[i].y - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;

        int idx = (gx + PADDING) + (gy + PADDING) * BUFFER_LINE;

        if (idx >= 0 && idx < (BUFFER_LINE * BUFFER_LINE)) {
            cells_ctr[idx] += 1;
        }
    }

    for(int i = 0, j = 0; i < (BUFFER_LINE * BUFFER_LINE); i++)
    {
        offsets[i] = j;
        j += cells_ctr[i];
    }
}

void computeIndicies()
{
    /*
    uint32_t* curr_pos = (uint32_t*)calloc(sizeof(uint32_t), POINTS_AMT);

    for(int i = 0; i < FLOATER_AMT; i++)
    {

        int x = (floaters[i].x-BUFFER_PADDING)/DISTANCE_BETWEEN_POINTS;
        int y = (floaters[i].y-BUFFER_PADDING)/DISTANCE_BETWEEN_POINTS;

        int idx = x + y*POINTS_WIDTH;

        int offset = offsets[idx];

        particles_loc[offset + curr_pos[idx]] = i;

        curr_pos[idx] += 1;

    }


    free(curr_pos);

    */

    uint32_t* curr_pos = (uint32_t*)calloc(sizeof(uint32_t), BUFFER_LINE * BUFFER_LINE);

    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int gx = (floaters[i].x - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;
        int gy = (floaters[i].y - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;

        int idx = (gx + PADDING) + (gy + PADDING) * BUFFER_LINE;

        if (idx >= 0 && idx < (BUFFER_LINE * BUFFER_LINE)) {
            int offset = offsets[idx];
            particles_loc[offset + curr_pos[idx]] = i;
            curr_pos[idx] += 1;
        }
    }
    free(curr_pos);
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

void initFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floaters[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
        floaters[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;

        floaters[i].v_x = ((rand() % 50) - 25)/10;
        floaters[i].v_y = ((rand() % 50) - 25)/10;

        floaters[i].r = (rand() % 127)+128;
        floaters[i].b = (rand() % 127)+128;
    }
}

void initGrid()
{
    int  dx[13]  =  {0,  1,  -1,  0,  0,   1,  1,   -1,  -1,  2,  -2,  0,  0};
    int  dy[13]  =  {0,  0,   0,  1, -1,   1, -1,    1,  -1,  0,   0,  2, -2};

    int relative_offsets[13];
    for(int i = 0; i < 13; i++) {
        

        relative_offsets[i] = dx[i] + (dy[i] * BUFFER_LINE);
    }

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

        for(int j = 0; j < 13; j++)
        {
            points[i].regions[j] = base_cell_id + relative_offsets[j];
        }
    }

}


void drawFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int idx = floaters[i].x*BYTES_PER_PIXEL + floaters[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
        static_rgb_buffer[idx]   = floaters[i].r;
        static_rgb_buffer[idx+1] = floaters[i].b;
    }
}

void simulateFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floaters[i].x += floaters[i].v_x;
        floaters[i].y += floaters[i].v_y;


        if (floaters[i].x < (float) BUFFER_PADDING) 
        {
            floaters[i].v_x *= -1.0f;
            floaters[i].x = BUFFER_PADDING;
        } 
        
       
        else if (floaters[i].x > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floaters[i].v_x *= -1.0f; 
            floaters[i].x = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }


        if (floaters[i].y < (float) BUFFER_PADDING) 
        {
            floaters[i].v_y *= -1.0f; 
            floaters[i].y = BUFFER_PADDING;
        }
        
        else if (floaters[i].y > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floaters[i].v_y *= -1.0f; 
            floaters[i].y = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }
    }
}

void drawConnections()
{
    int offset = DISTANCE_BETWEEN_POINTS/2;    

    const int SQR_PER_ROW = POINTS_WIDTH - 1;
    const int SQR_PER_COL = POINTS_HEIGHT - 1;

    /*
    for(int i = 0; i < SQR_PER_COL; i++)
    {
        for(int j = 0; j < SQR_PER_ROW; j++)
        {
            int idx = POINTS_WIDTH*i+j;

            float strength = 0.0f;

            int x = points[idx].i_x;
            int y = points[idx].i_y;

            for(int k = 0; k < FLOATER_AMT; k++)
            {
                float val = (floaters[k].r+floaters[k].b)/((floaters[k].x-x)*(floaters[k].x-x)+(floaters[k].y-y)*(floaters[k].y-y));
                strength += val;
            }

            points[idx].strength = strength;

            //std::cout << (strength > THRESHOLD) ? '1' : ' ';

        }
    }
    */

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
                int floater_idx = particles_loc[idx_o + k];

                float dx = floaters[floater_idx].x - x;
                float dy = floaters[floater_idx].y - y;
                float dist_sq = dx*dx + dy*dy;

                if(dist_sq < 0.001f) dist_sq = 0.001f;

                float val = (floaters[floater_idx].r + floaters[floater_idx].b) / dist_sq;

                //float val = (floaters[particles_loc[idx_o+k]].r+floaters[particles_loc[idx_o+k]].b)/((floaters[particles_loc[idx_o+k]].x-x)*(floaters[particles_loc[idx_o+k]].x-x)+(floaters[particles_loc[idx_o+k]].y-y)*(floaters[particles_loc[idx_o+k]].y-y));

                strength += val;
            }

        }

        points[i].strength = (uint16_t)strength;
    }
    //std::cout << '\n';

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
