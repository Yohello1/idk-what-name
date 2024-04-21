#include <SDL2/SDL.h>
#include <opencv4/opencv2/opencv.hpp>
#include <sycl/sycl.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//sf::RenderWindow window(sf::VideoMode(1024, 1024), "draw2");

int main(int argc, char** argv)
{
    std::cout << "Hello world" << std::endl;

    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 1024, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    int width, height, channels;
    unsigned char* img = stbi_load("webcam_output.jpeg", &width, &height, &channels, 4);
    if (img == 0)
    {
        std::cout << "Error loading image file" << std::endl;
        return -1;
    }



    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_Quit();

    return 0;

}
