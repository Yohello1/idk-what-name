#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#define STB_TRUETYPE_IMPLEMENTATION // force following include to generate implementation
#include "jeux-video/ui/stb_truetype.h"
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024

// time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
enum pixel_state
{
    empty,
    gas,
    fluid,
    solid,
    fire,
    burning,
    burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    uint8_t r, g, b, a;
    uint16_t temperature;
    uint8_t pressure;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void redraw_and_render();

// This is deff wrong
unsigned char screen[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
unsigned char file_buffer[24 << 20];

// this script is gonn hurtme

int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << '\n';
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);
    // PAINT IT BLACK

    stbtt_fontinfo font;
    // Look at the picture for understanding?
    // `ch` indicates the charcter it is currently
    int ascent, baseline, ch = 0;
    float scale, xpos = 2;

    // I dont want to use a char star but ehhh
    // char *text = "Hi I am a bagguette";
    std::vector<char> text_2_render; // "Hello World";

    {
        char text_2_add[] = "Hallo World";
        for (int i = 0; i < (int)strlen(text_2_add); i++)
        {
            text_2_render.push_back(text_2_add[i]);
            std::cout << text_2_render[i];
        }
    }
    std::cout << '\n';

    // Reading the ttf file into memory
    fread(file_buffer, 1, 1000000, fopen("jeux-video/ui/font/Hack-Regular.ttf", "rb"));
    stbtt_InitFont(&font, file_buffer, 0);

    // Setting the font size?
    // Do some future testing with this
    scale = stbtt_ScaleForPixelHeight(&font, 30);
    stbtt_GetFontVMetrics(&font, &ascent, 0, 0);

    // I havent a clue what this does
    // Check the image
    baseline = (int)(ascent * scale);

    while (text_2_render[ch])
    {
        // Get the cords for evetyhing
        int advance, lsb, x0, y0, x1, y1;

        // not a single clue what this is supposed to do lol
        float x_shift = xpos - (float)floor(xpos);

        // Gets the data for this specific chacter
        stbtt_GetCodepointHMetrics(&font, text_2_render[ch], &advance, &lsb);

        // This tells us when the actual charcter starts & stops
        stbtt_GetCodepointBitmapBoxSubpixel(&font, text_2_render[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

        // Actually encode it?
        stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, LOGICAL_WINDOW_WIDTH, scale, scale, x_shift, 0, text_2_render[ch]);

        xpos += (advance * scale);
        if (text_2_render[ch + 1])
        {
            xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
        }
        ch++;
    }

    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            // Bit shift it a tad bit, and yeah
            if ((int)screen[y_pos][x_pos] >> 5 > 2)
            {
                // Without the minus one, none of this works lol
                pixels[x_pos][y_pos].r = ((int)screen[y_pos][x_pos] >> 5)*((255/7) - 1);
                pixels[x_pos][y_pos].g = ((int)screen[y_pos][x_pos] >> 5)*((255/7) - 1);
                pixels[x_pos][y_pos].b = ((int)screen[y_pos][x_pos] >> 5)*((255/7) - 1);
                pixels[x_pos][y_pos].a = ((int)screen[y_pos][x_pos] >> 5)*((255/7) - 1);
            }
        }
    }

    redraw_and_render();

    excecution_finished();
}

void excecution_finished(void)
{
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }
        SDL_RenderPresent(renderer);
        // SDL_Delay(50);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void redraw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            // if (pixels[x_pos][y_pos].state_now == solid)
            // {
            //     SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            //     SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            // }
        }
    }
}
