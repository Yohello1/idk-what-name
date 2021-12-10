
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <iostream>
#include <random>
#include <cmath>
#define LOGICAL_WINDOW_WIDTH 512
#define ACTUAL_WINDOW_WIDTH 512

struct positions
{
    int y_pos;
    int x_pos;
    int r;
    int g;
    int b;
};
int noise_run = 50;
int currenttime = (unsigned int)time(NULL);

int main(void)
{
    int upscaling = 1;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    std::cout << "Time = " << currenttime << "\n";
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    int amount_of_points = 500;
    struct positions spots[amount_of_points];
    srand(currenttime + noise_run);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (int positions_to_generate = 0; positions_to_generate < amount_of_points; positions_to_generate++)
    {
        spots[positions_to_generate].x_pos = rand() % LOGICAL_WINDOW_WIDTH;
        spots[positions_to_generate].y_pos = rand() % LOGICAL_WINDOW_WIDTH;
        spots[positions_to_generate].r = rand() % 255;
        spots[positions_to_generate].g = rand() % 255;
        spots[positions_to_generate].b = rand() % 255;
        std::cout << "Should be generating " << positions_to_generate << "\n"
                  << "X = " << spots[positions_to_generate].x_pos << "\n"
                  << "Y = " << spots[positions_to_generate].y_pos << "\n";
    }
    std::cout << "Should be done generating\n";
    for (int i = 0; i < amount_of_points; ++i)
    {
        SDL_SetRenderDrawColor(renderer, spots[i].r, spots[i].g, spots[i].b, 255);
        SDL_RenderDrawPoint(renderer, spots[i].x_pos, spots[i].y_pos);
        // SDL_Delay(50);
        SDL_RenderPresent(renderer);
    }
    SDL_RenderPresent(renderer);
    std::cout << "Done making points\n";

    // for(int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)

    for (int y = 0; y < LOGICAL_WINDOW_WIDTH; y++)
    {
        for (int x = 0; x < LOGICAL_WINDOW_WIDTH; x++)
        {
            int point;
            float distance = 1000000000;
            for (int i = 0; i < amount_of_points; i++)
            {
                int y_dis = spots[i].y_pos - y;
                int x_dis = spots[i].x_pos - x;
                float last_distance = sqrt(pow(y_dis, 2) + pow(x_dis, 2));
                // mvprintw(20, 3, "%.2f", last_distance);
                // mvprintw(21, 3, "%.2f", distance);

                if (last_distance < distance)
                {
                    distance = last_distance;
                    point = i;
                    // mvprintw(19, 3, "%i", point);
                    // mvprintw(18, 3, "%i", updates);
                }
            }
            SDL_SetRenderDrawColor(renderer, spots[point].r, spots[point].g, spots[point].b, 255);

            SDL_RenderDrawPoint(renderer, x, y);
            // std::cout << "Point (" << x << " , " << y << ") \n";

            // getch();
        }
        SDL_RenderPresent(renderer);

        // getch();
    }
    SDL_RenderPresent(renderer);

    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            break;
        }

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
