// #include <..base_data.hpp>
#include "stb_truetype.h"
#define STB_TRUETYPE_IMPLEMENTATION
char buffer[24 << 20];
stbtt_fontinfo font;
int i, j, ascent, ch = 0;

/*
Oi listen up


Ui
|
|---> Changing Text
|---> Boxes
|---> Fixed text
|---> Fixed boxes
*/

namespace ui
{
    void draw_ui()
    {
        // wut
        // I am not smort
    }

    void init_text(const char *font_path)
    {
        // fread(buffer, 1, 1000000, fopen(font_path, "rb"));
        // stbtt_InitFont(&font, buffer, 0);
    }

    class boxes
    {
    private:
        int higher_x = 0;
        int higher_y = 0;
        int lower_x = 0;
        int lower_y = 0;
        colour box_colour;

    public:
        boxes(cord_2d cord_1, cord_2d cord_2, colour colour_new)
        {
            // perm = perm_new;
            lower_x = std::min(cord_1.x_pos, cord_2.x_pos);
            lower_y = std::min(cord_1.y_pos, cord_2.y_pos);
            higher_x = std::max(cord_1.x_pos, cord_2.x_pos);
            higher_y = std::max(cord_1.y_pos, cord_2.y_pos);
            box_colour = colour_new;
        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
            for (int x_pos = lower_x; x_pos < higher_x; x_pos++)
            {
                for (int y_pos = lower_y; y_pos < higher_y; y_pos++)
                {
                    render[x_pos][y_pos].r = box_colour.r;
                    render[x_pos][y_pos].g = box_colour.g;
                    render[x_pos][y_pos].b = box_colour.b;
                    render[x_pos][y_pos].a = box_colour.a;
                    std::cout << "Hellow rodl" << '\n';
                }
            }
        }

        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }
    };

    class text
    {
    private:
        // we're not gonna deal with permanent things yet
        // bool perm = false;
        colour box_colour;

        int baseline;
        float scale, xpos = 2;
        std::vector<char> text_to_render;

        int higher_x = 0;
        int higher_y = 0;
        int lower_x = 0;
        int lower_y = 0;

    public:
        text(cord_2d cord_1, cord_2d cord_2, colour colour_new, std::string text_to_render_new)
        {
            std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_to_render));
            // perm = perm_new;
            lower_x = std::min(cord_1.x_pos, cord_2.x_pos);
            lower_y = std::min(cord_1.y_pos, cord_2.y_pos);
            higher_x = std::max(cord_1.x_pos, cord_2.x_pos);
            higher_y = std::max(cord_1.y_pos, cord_2.y_pos);
            box_colour = colour_new;
        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
        }
        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }
    };

}