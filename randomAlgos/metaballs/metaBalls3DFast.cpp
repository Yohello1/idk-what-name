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

constexpr int BUFFER_WIDTH = 1280/2;
constexpr int BUFFER_HEIGHT = 720/2;
constexpr int BYTES_PER_PIXEL = 3; // R, G, B
constexpr int SCREEN_SCALE = 1;

const float META_BALL_SURFACE_MIN = 150.0;
const float META_BALL_SURFACE_MAX = 2300.0;
const uint32_t META_BALLS_AMT = 2000;
const uint32_t META_BALL_STRENGTH_MAX = 6000;
const uint32_t META_BALL_STRENGTH_MIN = 5000;
const uint32_t META_BALL_SPAWN_AREA = 500;

struct s_metaBall {
    float x_p;
    float y_p;
    float z_p;
    float x_v;
    float y_v;
    float z_v;

    float strength;
    float _padding;
};
typedef struct s_metaBall s_metaBall;

s_metaBall* g_metaBalls = new s_metaBall[META_BALLS_AMT];

const float VIEW_MIN_DIST = 1.0f;
const float MAX_DIST = 500.0f;
const int MAX_STEPS = 256;

sycl::device getBestDevice() {
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

namespace meatBalls
{
    glm::vec3 g_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 g_cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f));
    const glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float g_yaw = -0.0f;
    float g_pitch = 90.0f;
    const float g_maxDist = 300.0f;
    const float g_minDist = 1.0f;
    const float g_fov = 1.370; // In radians

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        front.y = sin(glm::radians(g_pitch));
        front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
        g_cameraFront = glm::normalize(front);
    }

    inline void moveForward(float ratio)
    {
        g_cameraPos += ratio * g_cameraFront;
        updateCameraVectors();
    }

    inline void moveBack(float ratio)
    {
        g_cameraPos -= ratio * g_cameraFront;
        updateCameraVectors();
    }

    inline void moveLeft(float ratio)
    {
        g_cameraPos += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        updateCameraVectors();
    }

    inline void moveRight(float ratio)
    {
        g_cameraPos -= glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * ratio;
        updateCameraVectors();
    }

    inline void moveUp(float ratio)
    {
        g_cameraPos += ratio * g_cameraUp;
        updateCameraVectors();

    }

    inline void rotateLeft(float ratio)
    {
        g_yaw -= ratio;
        updateCameraVectors();
    }

    inline void rotateRight(float ratio)
    {
        g_yaw += ratio;
        updateCameraVectors();
    }

    inline void rotateUp(float ratio)
    {
        g_pitch += ratio;
        g_pitch = std::fmin(89.0f, g_pitch);
        updateCameraVectors();
    }

    inline void rotateDown(float ratio)
    {
        g_pitch -= ratio;
        g_pitch = std::fmax(89.0f, g_pitch);
        updateCameraVectors();
    }
}

void handleControls(SDL_Event* event, bool* quit)
{

    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            *quit = true;
        }
    }


    const uint8_t* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_W])
    {
        meatBalls::moveForward(5.0f);
        std::cout << "bai" << std::endl;
    }
    if(state[SDL_SCANCODE_S])
    {
        meatBalls::moveBack(5.0f);
    }
    if(state[SDL_SCANCODE_A])
    {
        meatBalls::moveRight(5.0f);
    }
    if(state[SDL_SCANCODE_D])
    {
        meatBalls::moveLeft(5.0f);
    }
    if(state[SDL_SCANCODE_E])
    {
        meatBalls::rotateLeft(3.0f);
    }
    if(state[SDL_SCANCODE_Q])
    {
        meatBalls::rotateRight(3.0f);
    }
}

void initMetaBalls()
{
    for(uint32_t i = 0; i < META_BALLS_AMT; i++)
    {
        g_metaBalls[i].x_p = (float) ((rand() % META_BALL_SPAWN_AREA) - (META_BALL_SPAWN_AREA/2));
        g_metaBalls[i].y_p = (float) ((rand() % META_BALL_SPAWN_AREA) - (META_BALL_SPAWN_AREA/2));
        g_metaBalls[i].z_p = (float) ((rand() % META_BALL_SPAWN_AREA) - (META_BALL_SPAWN_AREA/2));

        g_metaBalls[i].x_v = (float) ((rand() % 1000) - 500)/1000;
        g_metaBalls[i].y_v = (float) ((rand() % 1000) - 500)/1000;
        g_metaBalls[i].z_v = (float) ((rand() % 1000) - 500)/1000;

        g_metaBalls[i].strength = (rand() % (META_BALL_STRENGTH_MAX - META_BALL_STRENGTH_MIN)) + META_BALL_STRENGTH_MIN;
    }
}

void drawMetaBalls(uint8_t* screen_buffer, const s_metaBall* balls, sycl::queue& sycl_queue)
{
    glm::vec3 w = glm::normalize(-1.0f * meatBalls::g_cameraFront);
    glm::vec3 u = glm::normalize(glm::cross(meatBalls::g_cameraUp, w));
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

        glm::u8vec3 final_colour;
        float hit_distance = MAX_DIST;
        bool hit_found = false;

        float t = VIEW_MIN_DIST;
        int step_count = 0;

        while (t < MAX_DIST && step_count < MAX_STEPS)
        {
            float total = 0.0f;
            glm::vec3 curr = t*ray_direction + ray_origin;

            for(int k = 0; k < META_BALLS_AMT; k++)
            {
                glm::vec3 diff = curr - glm::vec3(balls[k].x_p, balls[k].y_p, balls[k].z_p);
                float dist_sq = glm::dot(diff, diff);
                total += balls[k].strength / dist_sq;
            }

            if (total > META_BALL_SURFACE_MIN)
            {
                hit_distance = t;
                hit_found = true;
                break;
            }

            t += 1.0f;
            step_count++;
        }

        glm::u8vec3 hit_colour;
        {
            float dist = glm::length(hit_distance * ray_direction);
            glm::u8vec3 temp(1,1,1);
            temp *= (float)(~((uint8_t) sycl::fabs(dist / (MAX_DIST / 256.0f))));
            hit_colour = temp;
        }

        glm::u8vec3 background_colour;
        {
            const glm::vec3 V_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
            glm::vec3 V_INPUT_NORMALIZED = glm::normalize(ray_direction);
            float cosTheta = glm::dot(V_INPUT_NORMALIZED, V_DOWN);
            float angleRadians = sycl::acos(cosTheta);

            const float MAX_ANGLE_RADIANS = glm::pi<float>();
            float t_bg = angleRadians / MAX_ANGLE_RADIANS;
            t_bg = sycl::clamp(t_bg, 0.0f, 1.0f);

            const float R_MAX = 78.0f;
            const float G_MAX = 179.0f;
            const float B_MAX = 252.0f;

            float r = R_MAX * t_bg;
            float g = G_MAX * t_bg;
            float b = B_MAX * t_bg;

            background_colour = glm::u8vec3((uint8_t) sycl::round(r), (uint8_t) sycl::round(g), (uint8_t) sycl::round(b));
        }

        final_colour = hit_found ? hit_colour : background_colour;

        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL   ] = final_colour.x;
        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL + 1] = final_colour.y;
        screen_buffer[i*BUFFER_WIDTH*BYTES_PER_PIXEL + j*BYTES_PER_PIXEL + 2] = final_colour.z;

    }).wait();
}

void simulateMetaBalls(s_metaBall* balls, sycl::queue sycl_queue)
{
    // This is bugged and doesnt work for things that exceed the boundary in the positive direction, will find solution
    sycl_queue.parallel_for(sycl::range<1>(META_BALLS_AMT), [=](sycl::id<1> idx)
    {
        uint32_t i = idx; 
        balls[i].x_p += balls[i].x_v;
        balls[i].y_p += balls[i].y_v;
        balls[i].z_p += balls[i].z_v;

        balls[i].x_v *= std::signbit(balls[i].x_p);
        balls[i].x_p = std::fmax(balls[i].x_p, 1.0);
        balls[i].x_p = std::fmin(balls[i].x_p, META_BALL_SPAWN_AREA);

        balls[i].y_v *= std::signbit(balls[i].x_p);
        balls[i].y_p = std::fmax(balls[i].y_p, 1.0);        
        balls[i].y_p = std::fmin(balls[i].y_p, META_BALL_SPAWN_AREA);

        balls[i].z_p = std::fmax(balls[i].z_p, 1.0);
        balls[i].z_p = std::fmin(balls[i].z_p, META_BALL_SPAWN_AREA);
        balls[i].z_v *= std::signbit(balls[i].z_p);
    }).wait();
}

int main(int argc, char* argv[]) {
    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    //omp_set_num_threads(16);
    srand(time(NULL));
    initMetaBalls();

    sycl::device my_device = getBestDevice();
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

    meatBalls::rotateRight(1.0f);


    // 6. Main application loop
    while (!quit) {
        // Handle events

        handleControls(&e, &quit);

        std::cout << "hi" << std::endl;

        drawMetaBalls(static_rgb_buffer, g_metaBalls, sycl_queue);
        simulateMetaBalls(g_metaBalls, sycl_queue);

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
