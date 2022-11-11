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
        hsv out;
        double min, max, delta;

        min = pixel_in.r < pixel_in.g ? pixel_in.r : pixel_in.g;
        min = min < pixel_in.b ? min : pixel_in.b;

        max = pixel_in.r > pixel_in.g ? pixel_in.r : pixel_in.g;
        max = max > pixel_in.b ? max : pixel_in.b;

        out.v = max; // v
        delta = max - min;
        if (delta < 0.00001)
        {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if (max > 0.0)
        {                          // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max); // s
        }
        else
        {
            // if max is 0, then r = g = b = 0
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = NAN; // its now undefined
            return out;
        }
        if (pixel_in.r >= max)                   // > is bogus, just keeps compilor happy
            out.h = (pixel_in.g - pixel_in.b) / delta; // between yellow & magenta
        else if (pixel_in.g >= max)
            out.h = 2.0 + (pixel_in.b - pixel_in.r) / delta; // between cyan & yellow
        else
            out.h = 4.0 + (pixel_in.r - pixel_in.g) / delta; // between magenta & cyan

        out.h *= 60.0; // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    }

    struct rgb
    HSV2RGB(hsv hsv_in)
    {
        // literally copied of some project, Im not doing the meaht to figure it out

        float h = hsv_in.h * 255 * 2.0f;   // 0-360
        float s = hsv_in.s * 255 / 255.0f; // 0.0-1.0
        float v = hsv_in.v * 255 / 255.0f; // 0.0-1.0

        float r, g, b; // 0.0-1.0

        int hi = (int)(h / 60.0f) % 6;
        float f = (h / 60.0f) - hi;
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (hi)
        {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        case 5:
            r = v, g = p, b = q;
            break;
        }
        rgb temp;

        temp.r = (unsigned char)(r * 255); // dst_r : 0-255
        temp.g = (unsigned char)(g * 255); // dst_r : 0-255
        temp.b = (unsigned char)(b * 255); // dst_r : 0-255

        // std::cout << "R : " << R << endl;
        // std::cout << "G : " << G << endl;
        // std::cout << "B : " << B << endl;
        return temp;
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
                // {
                //     double fade = pixels[x_pos][y_pos].fetch_pressure() / 65536;
                //     data.s = (data.s - (data.s * (fade * 100)));
                // }

                rgb temp = Render::HSV2RGB(data);

                SDL_SetRenderDrawColor(renderer, rgb_cell.r, rgb_cell.g, rgb_cell.b, 255);
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