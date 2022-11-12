// #include "base_data.hpp"
/**
 * @brief IMPORTANT: COLOURS ARE HANDLED VERY DIFFERENTLY IN THIS FILE
 *
 */
namespace Render
{

    typedef struct
    {
        double r; // a fraction between 0 and 1
        double g; // a fraction between 0 and 1
        double b; // a fraction between 0 and 1
    } rgb;

    typedef struct
    {
        double h; // angle in degrees
        double s; // a fraction between 0 and 1
        double v; // a fraction between 0 and 1
    } hsv;

    static hsv rgb2hsv(rgb in);
    static rgb hsv2rgb(hsv in);

    hsv rgb2hsv(rgb in)
    {
        hsv out;
        double min, max, delta;

        min = in.r < in.g ? in.r : in.g;
        min = min < in.b ? min : in.b;

        max = in.r > in.g ? in.r : in.g;
        max = max > in.b ? max : in.b;

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
        if (in.r >= max)                   // > is bogus, just keeps compilor happy
            out.h = (in.g - in.b) / delta; // between yellow & magenta
        else if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan

        out.h *= 60.0; // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    }

    rgb hsv2rgb(hsv in)
    {
        double hh, p, q, t, ff;
        long i;
        rgb out;

        if (in.s <= 0.0)
        { // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i)
        {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
        }
        return out;
    }

    void redraw_render(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Renderer *renderer)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                // It'll get optimisied by the compiler anyways
                // pixels[x_pos][y_pos].change_b(std::min(pixels[x_pos][y_pos].fetch_b() + pixels[x_pos][y_pos].fetch_pressure(), 255));

                rgb rgb_cell;
                rgb_cell.r = pixels[x_pos][y_pos].fetch_r() / pow(2,8) ;
                rgb_cell.g = pixels[x_pos][y_pos].fetch_g() / pow(2,8) ;
                rgb_cell.b = pixels[x_pos][y_pos].fetch_b() / pow(2,8) ;

                hsv data = Render::rgb2hsv(rgb_cell);

                // Fading 
                {   
                    data.s = data.s - (data.s*(pixels[x_pos][y_pos].fetch_pressure()/pow(2,16)));
                }

                rgb temp = Render::hsv2rgb(data);

                temp.r *= pow(2,8);
                temp.g *= pow(2,8);
                temp.b *= pow(2,8);

                SDL_SetRenderDrawColor(renderer, temp.r , temp.g , temp.b , 255);
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