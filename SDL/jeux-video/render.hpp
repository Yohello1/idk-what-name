#include <SDL2/SDL.h>
// #include "base_data.hpp"
namespace Render
{
    void redraw_render(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Renderer *renderer)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }

    void draw_point(int x_pos, int y_pos, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Renderer *renderer)
    {
        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
    }
}