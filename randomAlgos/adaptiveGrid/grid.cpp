#include <SDL.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>

// --- Configuration ---
const int BOX_CNT = 24;
const int BOX_INIT_SIZE = 24;
const int BOX_TOTAL = BOX_CNT*BOX_CNT;

const int BUFFER_WIDTH = BOX_CNT*BOX_INIT_SIZE;
const int BUFFER_HEIGHT = BOX_CNT*BOX_INIT_SIZE;
const int BYTES_PER_PIXEL = 3; // R, G, B
const int SCREEN_SCALE = 1;

uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

typedef struct s_box
{
    int x_top;
    int y_top;
    int x_bot;
    int y_bot;
};

s_box boxBuffer[BOX_CNT * BOX_CNT];

void initBox()
{
    for(int i = 0; i < BOX_CNT; i++)
    {
        for(int j = 0; j < BOX_CNT; j++)
        {
            boxBuffer[i*BOX_CNT+j].x_top = i*BOX_INIT_SIZE;
            boxBuffer[i*BOX_CNT+j].y_top = j*BOX_INIT_SIZE;
            boxBuffer[i*BOX_CNT+j].x_bot = (i+1)*BOX_INIT_SIZE;
            boxBuffer[i*BOX_CNT+j].y_bot = (j+1)*BOX_INIT_SIZE;
            std::cout << '(' << i << ',' << j << ") ";
        }
    }
}

void drawBox()
{
    for(int i = 0; i < BOX_TOTAL; i++)
    {
        s_box& box = boxBuffer[i];
        
        for (int x = box.x_top; x < box.x_bot; ++x) {
            if (x >= 0 && x < BUFFER_WIDTH && box.y_top >= 0 && box.y_top < BUFFER_HEIGHT) {
                int idx = (box.y_top * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
                static_rgb_buffer[idx] = 255;
                static_rgb_buffer[idx + 1] = 255;
                static_rgb_buffer[idx + 2] = 255;
            }
        }
        for (int x = box.x_top; x < box.x_bot; ++x) {
            if (x >= 0 && x < BUFFER_WIDTH && box.y_bot - 1 >= 0 && box.y_bot - 1 < BUFFER_HEIGHT) {
                int idx = ((box.y_bot - 1) * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
                static_rgb_buffer[idx] = 255;
                static_rgb_buffer[idx + 1] = 255;
                static_rgb_buffer[idx + 2] = 255;
            }
        }
        for (int y = box.y_top; y < box.y_bot; ++y) {
            if (y >= 0 && y < BUFFER_HEIGHT && box.x_top >= 0 && box.x_top < BUFFER_WIDTH) {
                int idx = (y * BUFFER_WIDTH + box.x_top) * BYTES_PER_PIXEL;
                static_rgb_buffer[idx] = 255;
                static_rgb_buffer[idx + 1] = 255;
                static_rgb_buffer[idx + 2] = 255;
            }
        }
        for (int y = box.y_top; y < box.y_bot; ++y) {
            if (y >= 0 && y < BUFFER_HEIGHT && box.x_bot - 1 >= 0 && box.x_bot - 1 < BUFFER_WIDTH) {
                int idx = (y * BUFFER_WIDTH + (box.x_bot - 1)) * BYTES_PER_PIXEL;
                static_rgb_buffer[idx] = 255;
                static_rgb_buffer[idx + 1] = 255;
                static_rgb_buffer[idx + 2] = 255;
            }
        }
    }
}

// --- Drawing function that fills the static array ONCE ---
void InitializeStaticBuffer() {
    for (int y = 0; y < BUFFER_HEIGHT; ++y) {
        for (int x = 0; x < BUFFER_WIDTH; ++x) {
            int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
            uint8_t blue = 0; 
            uint8_t red = 0;
            uint8_t green = 0;
            static_rgb_buffer[buffer_index + 0] = red; 
            static_rgb_buffer[buffer_index + 1] = green;
            static_rgb_buffer[buffer_index + 2] = blue;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

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

    initBox();

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        drawBox();

        SDL_BlitSurface(bufferSurface, nullptr, screenSurface, nullptr);
        SDL_UpdateWindowSurface(window);

    }
    SDL_FreeSurface(bufferSurface); 
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
