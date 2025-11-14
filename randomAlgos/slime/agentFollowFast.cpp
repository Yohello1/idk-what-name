#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <sycl/sycl.hpp>
#include <numbers>


// --- Configuration ---
const int BUFFER_WIDTH = 1080;
const int BUFFER_HEIGHT = 1080;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

// This array will hold BUFFER_WIDTH * BUFFER_HEIGHT * 3 bytes (R, G, B)
// uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];
sycl::queue sycl_queue{sycl::gpu_selector_v};
uint8_t* static_rgb_buffer = sycl::malloc_shared<uint8_t>(BUFFER_WIDTH*BUFFER_HEIGHT*BYTES_PER_PIXEL, sycl_queue);

uint8_t* temp_rgb_buffer = sycl::malloc_shared<uint8_t>(BUFFER_WIDTH*BUFFER_HEIGHT*BYTES_PER_PIXEL, sycl_queue);

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

uint32_t agentsAmt = 50000;
agentData* agentsData = new agentData[agentsAmt]; 
float angleObserve = 45.0;
float turnFactor = 0.05;
float sensorDistance = 4.0f;

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
        agentsData[i].y_p = (rand() % BUFFER_HEIGHT/2)+BUFFER_HEIGHT/4;
        agentsData[i].x_v = ((rand() % 1000) - 500.0)/100.0;
        agentsData[i].y_v = ((rand() % 1000) - 500.0)/100.0;
        agentsData[i].r = 255;
        agentsData[i].g = 255;
        agentsData[i].b = 255;

        float speed = std::sqrt(agentsData[i].x_v*agentsData[i].x_v + agentsData[i].y_v*agentsData[i].y_v);
        
        agentsData[i].x_v /= speed;
        agentsData[i].y_v /= speed;
    
        agentsData[i].x_v *= 5.0;
        agentsData[i].y_v *= 5.0;
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
float vx_scaled = agentsData[i].x_v * sensorDistance;
        float vy_scaled = agentsData[i].y_v * sensorDistance;

        float x_45 = vx_scaled * std::cos(degToRad(angleObserve)) - vy_scaled * std::sin(degToRad(angleObserve));
        float y_45 = vx_scaled * std::sin(degToRad(angleObserve)) + vy_scaled * std::cos(degToRad(angleObserve));

        float x_v_45 = x_45 + agentsData[i].x_p;
        float y_v_45 = y_45 + agentsData[i].y_p;

        float x_45_i = vx_scaled * std::cos(-1*degToRad(angleObserve)) - vy_scaled * std::sin(-1*degToRad(angleObserve));
        float y_45_i = vx_scaled * std::sin(-1*degToRad(angleObserve)) + vy_scaled * std::cos(-1*degToRad(angleObserve));
        float x_v_45_i = x_45_i + agentsData[i].x_p;
        float y_v_45_i = y_45_i + agentsData[i].y_p;


        uint16_t l_score;
        uint16_t c_score; // center
        uint16_t r_score;

        float safe_x_l_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, x_v_45));
        float safe_y_l_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, y_v_45));

        float safe_x_c_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, agentsData[i].x_p + vx_scaled));
        float safe_y_c_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, agentsData[i].y_p + vy_scaled));


        float safe_x_r_45 = std::fmax(0.0f, std::fmin((float)BUFFER_WIDTH - 1.0f, x_v_45_i));
        float safe_y_r_45 = std::fmax(0.0f, std::fmin((float)BUFFER_HEIGHT - 1.0f, y_v_45_i)); // Removed the -1* multiplier
        


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
//static_rgb_buffer[i] = std::max((int)static_rgb_buffer[i], 3);
        }
    }

}

void applyMeanBlur(uint8_t* arr, uint8_t* temp) {
    // uint8_t* temp_buffer = (uint8_t*)calloc(BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL, 1);
    
    /*
    if (temp_buffer == nullptr) {
        std::cerr << "Memory allocation for temporary buffer failed!" << std::endl;
        return;
    }
    */

    const int KERNEL_SIZE = 5;
    const int KERNEL_RADIUS = 2; // = 1
    const int KERNEL_AREA = KERNEL_SIZE * KERNEL_SIZE; // =5

    // const size_t workSize = BUFFER_WIDTH * BUFFER_HEIGHT;


    sycl_queue.parallel_for(sycl::range<2>(BUFFER_WIDTH, BUFFER_HEIGHT) , [=](sycl::id<2> idx){

        size_t x = idx[0];
        size_t y = idx[1];
        
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

                sum_r += arr[neighbor_index + 0];
                sum_g += arr[neighbor_index + 1];
                sum_b += arr[neighbor_index + 2];
            }
        }

        uint8_t avg_r = (uint8_t)(sum_r / KERNEL_AREA);
        uint8_t avg_g = (uint8_t)(sum_g / KERNEL_AREA);
        uint8_t avg_b = (uint8_t)(sum_b / KERNEL_AREA);

        uint32_t output_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

        temp[output_index + 0] = avg_r;
        temp[output_index + 1] = avg_g;
        temp[output_index + 2] = avg_b;
        

    });
    
    sycl_queue.wait();

    /*
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
    } */

    std::memcpy(arr, temp, BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL);
    // free(temp_buffer);
}

// sycl code 

void InitializeStaticBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {
            
            int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
            static_rgb_buffer[buffer_index + 0] = 10; 
            static_rgb_buffer[buffer_index + 1] = 10;
            static_rgb_buffer[buffer_index + 2] = 10;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    srand(time(NULL));
    initAgents();

    


    InitializeStaticBuffer();

    SDL_Window* window = SDL_CreateWindow(
        "SDL Static C-Style Array Buffer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        BUFFER_WIDTH * SCREEN_SCALE, 
        BUFFER_HEIGHT * SCREEN_SCALE,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface* bufferSurface = SDL_CreateRGBSurfaceFrom(
        static_rgb_buffer,
        BUFFER_WIDTH,
        BUFFER_HEIGHT,
        24,
        BUFFER_WIDTH * BYTES_PER_PIXEL,
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0x00000000
    );

    if (bufferSurface == nullptr) {
        std::cerr << "Could not create buffer surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        applyMeanBlur(static_rgb_buffer, temp_rgb_buffer);
        drawAgents();
        simulateAgents();

        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);


        SDL_UpdateWindowSurface(window);

        //SDL_Delay(15);
    }

    SDL_FreeSurface(bufferSurface); 
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
