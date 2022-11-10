// #include "base_data.hpp"
namespace Render
{
    struct hsv
    {
        double h;
        double s;
        double v;
    };

    // I know I have a struct called colour, but that's for rgba
    // and Im a lazy bastard who doesnt wanna deal with cross-namespace stuff
    struct rgb
    {
        double r;
        double g;
        double b;
    };

    struct hsv RGB2HSV(rgb pixel_in)
    {
        double r = pixel_in.r;
        double g = pixel_in.g;
        double b = pixel_in.b;

        double K = 0.f;

        if (g < b)
        {
            std::swap(g, b);
            K = -1.f;
        }

        if (r < g)
        {
            std::swap(r, g);
            K = -2.f / 6.f - K;
        }

        double chroma = r - std::min(g, b);
        hsv temp;

        temp.h = fabs(K + (b - g / (6.f * chroma + 1e-20f)));
        temp.s = chroma / (r + 1e-20f);
        temp.v = r;

        return temp;
    }

    struct rgb
    HSV2RGB(hsv hsv_in)
    {
        double max = hsv_in.v * 255;
        double min = max * (1 - hsv_in.s);
    }

    void
    redraw_render(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Renderer *renderer)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                // It'll get optimisied by the compiler anyways
                // pixels[x_pos][y_pos].change_b(std::min(pixels[x_pos][y_pos].fetch_b() + pixels[x_pos][y_pos].fetch_pressure(), 255));

                rgb rgb_cell;
                rgb_cell.r = pixels[x_pos][y_pos].fetch_r();
                rgb_cell.g = pixels[x_pos][y_pos].fetch_g();
                rgb_cell.b = pixels[x_pos][y_pos].fetch_b();


                hsv data = Render::RGB2HSV(rgb_cell);

                // Fading for when it has a lot of pressure
                {
                    double fade = pixels[x_pos][y_pos].fetch_pressure() / 65536;
                    data.s = (data.s - (data.s * (fade * 100)));
                }

                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].fetch_r(), pixels[x_pos][y_pos].fetch_g(), pixels[x_pos][y_pos].fetch_b(), pixels[x_pos][y_pos].fetch_a());
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }

    void draw_point(int x_pos, int y_pos, cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Renderer *renderer)
    {
        SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].fetch_r(),
                               pixels[x_pos][y_pos].fetch_g(),
                               pixels[x_pos][y_pos].fetch_b(),
                               pixels[x_pos][y_pos].fetch_a());
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