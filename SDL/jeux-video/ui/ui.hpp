// #include <..base_data.hpp>

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

    stbtt_fontinfo font;
    unsigned char buffer[24 << 20];

    void draw_ui()
    {
        // wut
        // I am not smort
    }

    // void init_text(const char *font_path)
    // {
    //     fread(buffer, 1, 1000000, fopen(font_path, "rb"));
    //     stbtt_InitFont(&font, buffer, 0);
    // }

    class single_ui_element
    {
    public:
        virtual ~single_ui_element() = default;
        virtual void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
            std::cout << "Wrong function" << '\n';
        }

    protected:
        colour box_colour;
    };

    class boxes : public single_ui_element
    {
    private:
        int higher_x = 0;
        int higher_y = 0;
        int lower_x = 0;
        int lower_y = 0;

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
            // std::cout << "bagguette" << '\n';
            for (int x_pos = lower_x; x_pos < higher_x; x_pos++)
            {
                for (int y_pos = lower_y; y_pos < higher_y; y_pos++)
                {
                    render[x_pos][y_pos].r = box_colour.r;
                    render[x_pos][y_pos].g = box_colour.g;
                    render[x_pos][y_pos].b = box_colour.b;
                    render[x_pos][y_pos].a = box_colour.a;
                    // std::cout << "Hellow rodl" << '\n';
                }
            }
        }

        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }
    };

    class text : public single_ui_element
    {
    private:
        int baseline;
        float scale, xpos = 2;
        std::vector<char> text_2_render;
        unsigned char *screen = NULL;

        uint16_t higher_x = 0;
        uint16_t higher_y = 0;
        uint16_t lower_x = 0;
        uint16_t lower_y = 0;

        uint16_t horz = 0;
        uint16_t vert = 0;

        int i, j, ascent, ch = 0;

    public:
        text(cord_2d cord_1, cord_2d cord_2, colour colour_new, std::string text_to_render_new, const char *font_path)
        {
            std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_2_render));
            // perm = perm_new;
            lower_x = std::min(cord_1.x_pos, cord_2.x_pos);
            lower_y = std::min(cord_1.y_pos, cord_2.y_pos);
            higher_x = std::max(cord_1.x_pos, cord_2.x_pos);
            higher_y = std::max(cord_1.y_pos, cord_2.y_pos);
            box_colour = colour_new;

            horz = higher_x - lower_x;
            vert = higher_y - lower_y;

            screen = (unsigned char *)malloc(sizeof(unsigned char) * (horz * vert));

            std::cout << "Reading the file" << '\n';
            fread(buffer, 1, 1000000, fopen(font_path, "rb"));

            // We really should be giving the developer control
            // Over these vars but ehhhhh
            stbtt_InitFont(&font, buffer, 0);
            scale = stbtt_ScaleForPixelHeight(&font, 15);
            stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
            baseline = (int)(ascent * scale);
        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {

            while (text_2_render[ch])
            {
                // Cords for each thing
                int advance, lsb, x0, y0, x1, y1;

                // Gets the x_pos
                float x_shift = xpos - (float)floor(xpos);

                // Gets the data for this specific chacter
                stbtt_GetCodepointHMetrics(&font, text_2_render[ch], &advance, &lsb);

                // How big is it gonna be huh?
                stbtt_GetCodepointBitmapBoxSubpixel(&font, text_2_render[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

                // Actually encode it?
                // the math involved in the screen thingy
                // x_pos + (x_width*y_pos)
                // (baseline + y0) + (((int)xpos + x0)*horz)
                stbtt_MakeCodepointBitmapSubpixel(&font, &screen[(baseline + y0) + (((int)xpos + x0) * horz)], x1 - x0, y1 - y0, horz, scale, scale, x_shift, 0, text_2_render[ch]);

                // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
                // because this API is really for baking character bitmaps into textures. if you want to render
                // a sequence of characters, you really need to render each bitmap to a temp buffer, then
                // "alpha blend" that into the working buffer

                // This is some recursive shid, dont touch it OK
                xpos += (advance * scale);
                if (text_2_render[ch + 1])
                    xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
                ++ch;
            }

            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
                {
                    if (lower_x < x_pos && x_pos < higher_x && lower_y < y_pos && y_pos < higher_y)
                    {
                        if ((screen[x_pos + (horz * y_pos)] >> 5) > 3)
                        {
                            render[x_pos][y_pos].r = box_colour.r;
                            render[x_pos][y_pos].g = box_colour.g;
                            render[x_pos][y_pos].b = box_colour.b;
                            render[x_pos][y_pos].a = box_colour.a;
                        }
                    }
                }
            }
        }

        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }
    };

    void storeObject(std::shared_ptr<single_ui_element> const &ob, std::vector<std::shared_ptr<ui::single_ui_element>> objects)
    {
        objects.push_back(ob);
    }
}