#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <sycl/sycl.hpp>
#include <numbers>


// --- Configuration ---
const int BUFFER_WIDTH = 900;
const int BUFFER_HEIGHT = 900;
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

uint32_t agentsAmt = 5000;
agentData* agentsData = new agentData[agentsAmt]; 
float angleObserve = 45.0;
float turnFactor = 0.1;
float sensorDistance = 3.0f;

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
        agentsData[i].x_p = (rand() % BUFFER_WIDTH/2)+BUFFER_WIDTH/4;
        agentsData[i].y_p = (rand() % BUFFER_HEIGHT/2)+BUFFER_WIDTH/4;
        agentsData[i].x_v = ((rand() % 100) - 50.0)/100.0;
        agentsData[i].y_v = ((rand() % 500) - 50.0)/100.0;
        agentsData[i].r = 255;
        agentsData[i].g = 255;
        agentsData[i].b = 255;

    }
}

float degToRad(float deg)
{
    return deg* (std::numbers::pi / 180.0f);
}

void simulateAgents()
{
    for(int i = 0; i < agentsAmt; i++)
    {
        // x_v, y_v is straight ahead
        // use 2d rotation matrix to get the two vectors that are 45 degrees
        // left and right, then we extract data from there 

        float x_45 = agentsData[i].x_v * std::cos(degToRad(angleObserve)) - agentsData[i].y_v * std::sin(degToRad(angleObserve));
        float y_45 = agentsData[i].x_v * std::sin(degToRad(angleObserve)) - agentsData[i].y_v * std::cos(degToRad(angleObserve));
        float x_v_45 = (x_45 + agentsData[i].x_p)*sensorDistance;
        float y_v_45 = (y_45 + agentsData[i].y_p)*sensorDistance;

        float x_45_i = agentsData[i].x_v * std::cos(-1*degToRad(angleObserve)) - agentsData[i].y_v * std::sin(-1*degToRad(angleObserve));
        float y_45_i = agentsData[i].x_v * std::sin(-1*degToRad(angleObserve)) - agentsData[i].y_v * std::cos(-1*degToRad(angleObserve));
        float x_v_45_i = (x_45_i + agentsData[i].x_p)*sensorDistance;
        float y_v_45_i = (y_45_i + agentsData[i].y_p)*sensorDistance;


        uint16_t l_score;
        uint16_t c_score; // center 
        uint16_t r_score;
      
        float safe_x_l_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, x_v_45));
        float safe_y_l_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, y_v_45));
        
        float safe_x_c_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, (agentsData[i].x_v+agentsData[i].x_p)*sensorDistance));
        float safe_y_c_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, (agentsData[i].y_v+agentsData[i].y_p)*sensorDistance));
        
        float safe_x_r_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, x_v_45_i));
        float safe_y_r_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, -1*y_v_45_i));

        


        l_score = static_rgb_buffer[(uint32_t)safe_y_l_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_l_45*BYTES_PER_PIXEL+2]
                + static_rgb_buffer[(uint32_t)safe_y_l_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_l_45*BYTES_PER_PIXEL+1]
                + static_rgb_buffer[(uint32_t)safe_y_l_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_l_45*BYTES_PER_PIXEL];
        

        c_score = static_rgb_buffer[(uint32_t)safe_y_c_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_c_45*BYTES_PER_PIXEL+2]
                + static_rgb_buffer[(uint32_t)safe_y_c_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_c_45*BYTES_PER_PIXEL+1]
                + static_rgb_buffer[(uint32_t)safe_y_c_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_c_45*BYTES_PER_PIXEL];

        r_score = static_rgb_buffer[(uint32_t)safe_y_r_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_r_45*BYTES_PER_PIXEL+2]
                + static_rgb_buffer[(uint32_t)safe_y_r_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_r_45*BYTES_PER_PIXEL+1]
                + static_rgb_buffer[(uint32_t)safe_y_r_45*BUFFER_WIDTH*BYTES_PER_PIXEL + (uint32_t)safe_x_r_45*BYTES_PER_PIXEL];

        l_score = (l_score + c_score)/2;
        r_score = (r_score + c_score)/2;
       
        if(c_score < l_score && c_score < r_score)
        {
            float new_x_v = ((rand() % 1000)-500)/100;
            float new_y_v = ((rand() % 1000)-500)/100;

            float magnitutde = std::sqrt(new_x_v*new_x_v + new_y_v*new_y_v);
            float original_speed = std::sqrt(agentsData[i].x_v*agentsData[i].x_v + agentsData[i].y_v*agentsData[i].y_v);

            if(magnitutde > 1e-6)
            {
                agentsData[i].x_v = (new_x_v / magnitutde) * original_speed;
                agentsData[i].y_v = (new_y_v / magnitutde) * original_speed;
            }
        }
        else if(l_score > c_score)
        {
            float new_x_v = x_45*(turnFactor) + agentsData[i].x_v*(1-turnFactor);
            float new_y_v = y_45*(turnFactor) + agentsData[i].y_v*(1-turnFactor);

            float magnitutde = std::sqrt(new_x_v*new_x_v + new_y_v*new_y_v);
            float original_speed = std::sqrt(agentsData[i].x_v*agentsData[i].x_v + agentsData[i].y_v*agentsData[i].y_v);

            if(magnitutde > 1e-6)
            {
                agentsData[i].x_v = (new_x_v / magnitutde) * original_speed;
                agentsData[i].y_v = (new_y_v / magnitutde) * original_speed;
            }

        }
        else if(r_score > c_score)
        {
            float new_x_v = x_45_i*turnFactor + agentsData[i].x_v*(1-turnFactor);
            float new_y_v = y_45_i*turnFactor + agentsData[i].y_v*(1-turnFactor);

            float magnitutde = std::sqrt(new_x_v*new_x_v + new_y_v*new_y_v);
            float original_speed = std::sqrt(agentsData[i].x_v*agentsData[i].x_v + agentsData[i].y_v*agentsData[i].y_v);

            if(magnitutde > 1e-6)
            {
                agentsData[i].x_v = (new_x_v / magnitutde) * original_speed;
                agentsData[i].y_v = (new_y_v / magnitutde) * original_speed;
            }
        }

        float jitter_angle = ((float) rand() / RAND_MAX * 2.0f - 1.0f) * 0.1f;

        float current_vx = agentsData[i].x_v;
        float current_vy = agentsData[i].y_v;

        agentsData[i].x_v = current_vx * cos(jitter_angle) - current_vy * sin(jitter_angle);
        agentsData[i].y_v = current_vx * sin(jitter_angle) + current_vy * cos(jitter_angle);

        agentsData[i].x_p += agentsData[i].x_v;
        agentsData[i].y_p += agentsData[i].y_v;




        if(agentsData[i].x_p < 0)
        {
            agentsData[i].x_p = std::fmax(agentsData[i].x_p, 1.0);
            agentsData[i].x_v *= -1;
        }

        else if(agentsData[i].y_p < 0)
        {
            agentsData[i].y_p = std::fmax(agentsData[i].y_p, 1.0);
            agentsData[i].y_v *= -1;
        }
        else if (agentsData[i].x_p > BUFFER_WIDTH )
        {
            agentsData[i].x_p = std::fmin(agentsData[i].x_p, (float)BUFFER_WIDTH-1 );
            agentsData[i].x_v *= -1;
        }
        else if (agentsData[i].y_p > BUFFER_HEIGHT)
        {
            agentsData[i].y_p = std::fmin(agentsData[i].y_p, (float)BUFFER_HEIGHT -1);
            agentsData[i].y_v *= -1;
        }
    

       // std::cout << ' '  <<agentsData[i].x_p << ' ' << agentsData[i].x_v << '\n';
    }

    for(int i = 0; i < (BUFFER_HEIGHT*BUFFER_WIDTH*BYTES_PER_PIXEL); i++)
    {
        if(static_rgb_buffer[i] > 0)
        {
            static_rgb_buffer[i] -= 1;
            static_rgb_buffer[i] = std::max((int)static_rgb_buffer[i], 0);
        }
    }

}

void applyMeanBlur() {
    uint8_t* temp_buffer = (uint8_t*)calloc(BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL, 1);

    if (temp_buffer == nullptr) {
        std::cerr << "Memory allocation for temporary buffer failed!" << std::endl;
        return;
    }

    const int KERNEL_SIZE = 5;
    const int KERNEL_RADIUS = KERNEL_SIZE / 2; // = 1
    const int KERNEL_AREA = KERNEL_SIZE * KERNEL_SIZE; // = 9

    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {

            uint32_t sum_r = 0;
            uint32_t sum_g = 0;
            uint32_t sum_b = 0;

            // Iterate over the 3x3 kernel neighborhood
            for (int ky = -KERNEL_RADIUS; ky <= KERNEL_RADIUS; ++ky) {
                for (int kx = -KERNEL_RADIUS; kx <= KERNEL_RADIUS; ++kx) {

                    int nx = x + kx;
                    int ny = y + ky;

                    nx = std::max(0, std::min(nx, BUFFER_WIDTH - 1));
                    ny = std::max(0, std::min(ny, BUFFER_HEIGHT - 1));

                    uint32_t neighbor_index = (ny * BUFFER_WIDTH + nx) * BYTES_PER_PIXEL;

                    sum_r += static_rgb_buffer[neighbor_index + 0];
                    sum_g += static_rgb_buffer[neighbor_index + 1];
                    sum_b += static_rgb_buffer[neighbor_index + 2];
                }
            }

            uint8_t avg_r = (uint8_t)(sum_r / KERNEL_AREA);
            uint8_t avg_g = (uint8_t)(sum_g / KERNEL_AREA);
            uint8_t avg_b = (uint8_t)(sum_b / KERNEL_AREA);

            uint32_t output_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            temp_buffer[output_index + 0] = avg_r;
            temp_buffer[output_index + 1] = avg_g;
            temp_buffer[output_index + 2] = avg_b;
        }
    }

    std::memcpy(static_rgb_buffer, temp_buffer, BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL);
    free(temp_buffer);
}

// sycl code 

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
        applyMeanBlur();
        drawAgents();
        simulateAgents();


        // --- COPY (BLIT) STATIC BUFFER TO SCREEN ---
        // Copies the fixed gradient image onto the window surface.
        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);


        // 7. Update the window to display the fixed changes
        SDL_UpdateWindowSurface(window);

        SDL_Delay(8);
    }

    // 8. Clean up resources
    // We only free the SDL_Surface wrapper; the C-style array memory is managed by the program.
    SDL_FreeSurface(bufferSurface); 
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
