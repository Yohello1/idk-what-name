#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <sycl/sycl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <omp.h>


// --- Configuration ---
constexpr int BUFFER_WIDTH = 1280;
constexpr int BUFFER_HEIGHT = 720;
constexpr int BYTES_PER_PIXEL = 3; // R, G, B
constexpr int SCREEN_SCALE = 1;


// Sycl Data
sycl::device get_best_device() {
    sycl::device selected_device;
    int best_score = -1;

    auto all_devices = sycl::device::get_devices();
    for (const auto& dev : all_devices) {
        std::string vendor = dev.get_info<sycl::info::device::vendor>();
        int current_score = 0;

        if (dev.is_gpu() && (vendor.find("NVIDIA") != std::string::npos)) {
            current_score = 3;
        } else if (dev.is_gpu()) {
            current_score = 2;
        } else if (dev.is_cpu()) {
            current_score = 1;
        }

        if (current_score > best_score) {
            best_score = current_score;
            selected_device = dev;
        }
    }
    
    if (best_score == -1) {
        throw std::runtime_error("No suitable device found!");
    }
    return selected_device;
}


// Rendering Data
const float view_min_dist = 1.0f;
const float maxDist = 500.0f;

glm::u8vec3 castRay(glm::vec3 ray_direction);

namespace meatBalls 
{
   struct s_viewport_matricies
    {
        glm::mat4 p_view;
        glm::mat4 p_perspective;
    };

    glm::vec3 g_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 g_cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float g_yaw = -90.0f;
    float g_pitch = 90.0f;
    const float g_maxDist = 300.0f;
    const float g_minDist = 1.0f;
    float g_fov = 2.00; // 74 degrees 

    struct s_viewport_matricies g_viewport_properties;

    void generateViewMatrix()
    {
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        front.y = sin(glm::radians(g_pitch));
        front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));

        g_cameraFront = glm::normalize(front);
    }

    void move_forward(float ratio)
    {
        g_cameraPos += ratio * g_cameraFront;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
        updateCameraVectors();
    }

    void move_back(float ratio)
    {
        g_cameraPos -= ratio * g_cameraFront;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
        updateCameraVectors();
    }

    void move_left(float ratio)
    {
        g_cameraPos += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
        updateCameraVectors();
    }

    void move_right(float ratio)
    {
        g_cameraPos -= glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
        updateCameraVectors();
    }

    void move_up(float ratio)
    {
        g_cameraPos += ratio * g_cameraUp;
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
        updateCameraVectors();

    }


    void rotate_left(float ratio)
    {
        g_yaw -= ratio;


        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void rotate_right(float ratio)
    {
        g_yaw += ratio;

        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void rotate_up(float ratio)
    {
        g_pitch += ratio;
    
        if(g_pitch > 89.0f)
        {
            g_pitch = 89.0f;
        }

        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }

    void rotate_down(float ratio)
    {
        g_pitch -= ratio;

        if (g_pitch < -89.0f) 
        {
            g_pitch = -89.0f;
        }

        updateCameraVectors();
        g_viewport_properties.p_view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    }


    inline glm::u8vec3 skyColour(glm::vec3 value)
    {
        
        const glm::vec3 V_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 V_INPUT_NORMALIZED = glm::normalize(value);

        float cosTheta = glm::dot(V_INPUT_NORMALIZED, V_DOWN);
        float angleRadians = std::acos(cosTheta); 
        
        const float MAX_ANGLE_RADIANS = glm::pi<float>();
        float t = angleRadians / MAX_ANGLE_RADIANS;
        t = std::clamp(t, 0.0f, 1.0f); 

        const float R_MAX = 78.0f;
        const float G_MAX = 179.0f;
        const float B_MAX = 252.0f;
        
        float r = R_MAX * t;
        float g = G_MAX * t;
        float b = B_MAX * t;
        
        return glm::u8vec3(
            (uint8_t)std::round(r), 
            (uint8_t)std::round(g), 
            (uint8_t)std::round(b)
        );
    }

}

// Meta Balls data
const float metaBall_surface_min = 500.0;
const float metaBall_surface_max = 2300.0;
const uint32_t metaBalls_amt = 500;
const uint32_t metaBall_strength_max = 1500;
const uint32_t metaBall_strength_min = 500;
const uint32_t metaBall_spawn_area = 1000;

float* metaBall_strength_map = new float[BUFFER_WIDTH * BUFFER_HEIGHT];

struct s_metaBall {
    float x_p;
    float y_p;
    float z_p;
    float x_v;
    float y_v;
    float z_v;

    float strength;
};

typedef struct s_metaBall s_metaBall;

s_metaBall* metaBalls = new s_metaBall[metaBalls_amt];

void initMetaBalls()
{
    for(uint32_t i = 0; i < metaBalls_amt; i++)
    {
        metaBalls[i].x_p = (float) ((rand() % metaBall_spawn_area) - (metaBall_spawn_area/2));
        metaBalls[i].y_p = (float) ((rand() % metaBall_spawn_area) - (metaBall_spawn_area/2));
        metaBalls[i].z_p = (float) ((rand() % metaBall_spawn_area) - (metaBall_spawn_area/2));
        
        metaBalls[i].x_v = (float) ((rand() % 1000) - 500)/1000;
        metaBalls[i].y_v = (float) ((rand() % 1000) - 500)/1000;
        metaBalls[i].z_v = (float) ((rand() % 1000) - 500)/1000;

        metaBalls[i].strength = (rand() % (metaBall_strength_max - metaBall_strength_min)) + metaBall_strength_min;
    }
}

void controls(SDL_Event* event, bool* quit)
{
 
    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            *quit = true;
        }
    }


    const uint8_t* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_W])
    {
        meatBalls::move_forward(5.0f);
        std::cout << "bai" << std::endl;
    }
    if(state[SDL_SCANCODE_S])
    {    
        meatBalls::move_back(5.0f);
    }
    if(state[SDL_SCANCODE_A])
    {
        meatBalls::move_right(5.0f);
    }
    if(state[SDL_SCANCODE_D])
    {
        meatBalls::move_left(5.0f);
    }


    if(state[SDL_SCANCODE_E])
    {
        meatBalls::rotate_up(3.0f);
    }
    if(state[SDL_SCANCODE_Q])
    {
        meatBalls::rotate_down(3.0f);
    }
    if(state[SDL_SCANCODE_C])
    {
        meatBalls::rotate_left(3.0f);
    }
    if(state[SDL_SCANCODE_Z])
    {
        meatBalls::rotate_right(3.0f);
    }
}

void drawMetaBalls(uint8_t* screen_buffer, const s_metaBall* balls, sycl::queue& sycl_queue)
{
    glm::vec3 w = glm::normalize(-1.0f * meatBalls::g_cameraFront); // view direction
    glm::vec3 u = glm::normalize(glm::cross(meatBalls::g_cameraUp, w)); // right vector
    glm::vec3 v = glm::cross(w,u); // Up vector
    glm::vec3 ray_origin = meatBalls::g_cameraPos;

    float viewportHeight = 2*meatBalls::g_minDist * std::tan(meatBalls::g_fov/2.0f);
    float viewportWidth = viewportHeight * ((float)BUFFER_WIDTH/(float) BUFFER_HEIGHT);


    sycl_queue.parallel_for(sycl::range<2>(BUFFER_HEIGHT, BUFFER_WIDTH), [=](sycl::id<2> idx)
    {
        size_t i = idx[0];
        size_t j = idx[1];


        float u_p = (((2.0*(float) j + 0.5f)/((float) BUFFER_WIDTH)) - 1.0f) *(viewportWidth/2.0f);
        float v_p = -1.0 * (((2.0*(float) i + 0.5f)/((float) BUFFER_HEIGHT))- 1.0f) *(viewportHeight/2.0f);
        
        glm::vec3 ray_direction = -1.0f * meatBalls::g_minDist * w + u_p * u + v_p * v;
        ray_direction = glm::normalize(ray_direction);

        glm::u8vec3 colour;
        bool hit_found = false;
        
        float t = 1.0f;
        float total;

        while(t < maxDist)
        {
            total = 0.0f;
            glm::vec3 curr = t*ray_direction + ray_origin;

            for(int k = 0; k < metaBalls_amt; k++)
            {
                glm::vec3 diff = curr - glm::vec3(balls[k].x_p, balls[k].y_p, balls[k].z_p);
                float dist_sq = glm::dot(diff, diff);
                total += balls[k].strength / dist_sq;
            }

            if(total > metaBall_surface_min)
            {
                float dist = glm::length(t*ray_direction);
                glm::u8vec3 temp(1,1,1);
                temp *= (int) std::abs(dist);

                colour = temp;
                hit_found = true;
                break;
            }

            t += 1.0f;
        }

        if (!hit_found)
        {
            // This was the final 'return' path
            colour = meatBalls::skyColour(ray_direction); 
        }
        
        // --- END INLINED castRay LOGIC ---

        // This write buffer logic was already here, and now uses the
        // 'colour' variable set by the inlined code.
        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL    ] = colour.x;
        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL + 1] = colour.y;
        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL + 2] = colour.z;
    
    }).wait();
}

void simulateMetaBalls(s_metaBall* balls, sycl::queue sycl_queue)
{
#pragma omp parallel for num_threads(16)
    for(int i = 0; i < metaBalls_amt; i++)
    {
        balls[i].x_p += balls[i].x_v;
        balls[i].y_p += balls[i].y_v;
        balls[i].z_p += balls[i].z_v;

        if(balls[i].x_p < 0)
        {
            balls[i].x_p = std::fmax(balls[i].x_p, 1.0);
            balls[i].x_v *= -1;
        }

        else if (balls[i].x_p > BUFFER_WIDTH )
        {
            balls[i].x_p = std::fmin(balls[i].x_p, (float)BUFFER_WIDTH-1);
            balls[i].x_v *= -1;
        }
        
        if(balls[i].y_p < 0)
        {
            balls[i].y_p = std::fmax(balls[i].y_p, 1.0);
            balls[i].y_v *= -1;
        }
        else if (balls[i].y_p > BUFFER_HEIGHT)
        {
            balls[i].y_p = std::fmax(balls[i].y_p, 1.0);
            balls[i].y_v *= -1;
        }
        
        if(balls[i].z_p < 0)
        {
            balls[i].z_p = std::fmax(balls[i].z_p, 1.0);
            balls[i].z_v *= -1;
        }
        else if (balls[i].z_p > BUFFER_HEIGHT)
        {
            balls[i].z_p = std::fmax(balls[i].z_p, 1.0);
            balls[i].z_v *= -1;
        }
    }
}

int main(int argc, char* argv[]) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    omp_set_num_threads(16);
    srand(time(NULL));
    initMetaBalls();

    sycl::device my_device = get_best_device();
    sycl::queue sycl_queue(my_device);
    uint8_t* static_rgb_buffer = sycl::malloc_shared<uint8_t>(BUFFER_WIDTH*BUFFER_HEIGHT*BYTES_PER_PIXEL, sycl_queue);

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

    meatBalls::rotate_right(1.0f);


    // 6. Main application loop
    while (!quit) {
        // Handle events

        controls(&e, &quit);
    
        std::cout << "hi" << std::endl;

        drawMetaBalls(static_rgb_buffer, metaBalls, sycl_queue);
        simulateMetaBalls(metaBalls, sycl_queue);

        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);

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
