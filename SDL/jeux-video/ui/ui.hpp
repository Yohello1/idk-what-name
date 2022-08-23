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
    unsigned char file_buffer[24 << 20];

    void draw_ui()
    {
        // wut
        // I am not smort
    }

    void init_font_all(const char *font_path)
    {
        // Puts it into the file buffer
        fread(file_buffer, 1, 1000000, fopen(font_path, "rb"));
        stbtt_InitFont(&font, file_buffer, 0);
    }

    class single_ui_element
    {
    public:
    // java classes > c++ classes
    // BASICALLY, "makes a destructor", and make a virtual draw function
        virtual ~single_ui_element() = default;
        virtual void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
            std::cout << "Wrong function" << '\n';
        }

    protected:
        // this is the only inherited thing
        // The colour
        colour box_colour;
    };

    class boxes : public single_ui_element
    {
    private:
        // Its cords
        int higher_x = 0;
        int higher_y = 0;
        int lower_x = 0;
        int lower_y = 0;


    public:
        boxes(cord_2d cord_1, cord_2d cord_2, colour colour_new)
        {
            // perm = perm_new;
            // Does some funky stuff to figure out which ones are which
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
        int ascent, baseline, ch = 0;
        float scale, xpos = 2;
        colour txt_color;

        std::vector<char> text_2_render;

        uint8_t min_power = 0; 
        cord_2d location;
        unsigned char screen[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];

    public:
        // Put a space at the begining of the string, idk why but if you dont it breaks everything
        // like " hello" instead of "hello"
        text(cord_2d location_in, colour colour_new, std::string text_to_render_new, int font_size, int min_power_in)
        {
            // Why on bloody earth does this work
            // Copies the text from the string into a vector/array
            std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_2_render));

            // Sets the colour, and location
            txt_color = colour_new;
            location = location_in;

            // Size
            scale = stbtt_ScaleForPixelHeight(&font, font_size);

            // No clue
            stbtt_GetFontVMetrics(&font, &ascent, 0, 0);

            // Note: Find the funky math to do the thing
            // do I just subtract the scale from the higher x?
            // God knows what this does
            baseline = (int)(ascent * scale) + location.y_pos;
            std::cout << (int)location.x_pos << '\n';
            min_power = min_power_in;
            std::cout << "End text constructor thingy" << '\n';
        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {

            while (text_2_render[ch])
            {
                // Get the cords for evetyhing
                int advance, lsb, x0, y0, x1, y1;

                // not a single clue what this is supposed to do lol
                // add to move it right
                float x_shift = xpos - (float)floor(xpos) + location.x_pos;

                // Gets the data for this specific chacter
                stbtt_GetCodepointHMetrics(&font, text_2_render[ch], &advance, &lsb);

                // This tells us when the actual charcter starts & stops
                stbtt_GetCodepointBitmapBoxSubpixel(&font, text_2_render[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

                // Actually encode it?
                stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, LOGICAL_WINDOW_WIDTH, scale, scale, x_shift, 0, text_2_render[ch]);

                // fuck if I know
                xpos += (advance * scale);
                if (text_2_render[ch + 1])
                {
                    xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
                }
                ch++;
            }

            ch = 0;
            xpos = 2;

            // I can def speed this up
            // Doing the same sort of thing i do for input drawing would work
            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
                {
                    // Bit shift it a tad bit, and yeah
                    if ((int)screen[y_pos][x_pos] >> 5 > min_power)
                    {
#if BLEND_TEXT == 0
                        render[x_pos][y_pos].r = ((render[x_pos][y_pos].r * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.r / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5)));
                        render[x_pos][y_pos].g = ((render[x_pos][y_pos].g * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.g / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5)));
                        render[x_pos][y_pos].b = ((render[x_pos][y_pos].b * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.b / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5)));
                        render[x_pos][y_pos].a = 255;
#elif BLEND_TEXT == 1
                        render[x_pos][y_pos].r = txt_color.r;
                        render[x_pos][y_pos].g = txt_color.g;
                        render[x_pos][y_pos].b = txt_color.b;
                        render[x_pos][y_pos].a = 255;
#endif

                    }
                }
            }
        }

        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }

        ~text()
        {
            free(screen);
        }
    };

}