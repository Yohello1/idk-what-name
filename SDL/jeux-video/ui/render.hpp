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

    // Only draws to a canvas
    // void draw_box(cord_2d point1, cord_2d point2, rgba_colour colour, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    // {
    //     // TODO, makes this not a performance issue
    //     uint16_t lower_x = std::min(point1.x_pos, point1.x_pos);
    //     uint16_t lower_y = std::min(point1.y_pos, point1.y_pos);
    //     uint16_t higher_x = std::max(point2.x_pos, point2.x_pos);
    //     uint16_t higher_y = std::max(point2.y_pos, point2.y_pos);

    //     for (int x_pos = 0; lower_x < x_pos < higher_x; x_pos++)
    //     {
    //         for (int y_pos = 0; lower_y < y_pos < higher_y; y_pos++)
    //         {
    //             pixels[x_pos][y_pos].r = colour.red;
    //             pixels[x_pos][y_pos].g = colour.green;
    //             pixels[x_pos][y_pos].b = colour.blue;
    //             pixels[x_pos][y_pos].a = colour.alpha;

    //         }
    //     }
    // }
}