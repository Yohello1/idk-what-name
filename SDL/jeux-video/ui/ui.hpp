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

    // void init_text(const char *font_path)
    // {
    //     fread(buffer, 1, 1000000, fopen(font_path, "rb"));
    //     stbtt_InitFont(&font, buffer, 0);
    // }

    void init_font_all()
    {
        fread(file_buffer, 1, 1000000, fopen("ui/font/Hack-Regular.ttf", "rb"));
        stbtt_InitFont(&font, file_buffer, 0);
    }

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
        int ascent, baseline, ch = 0;
        float scale, xpos = 2;

        // std::vector<char> text_2_render;
        char text_2_render[11] = "I am tired";

        /*
                uint16_t higher_x = 0;
                uint16_t higher_y = 0;
                uint16_t lower_x = 0;
                uint16_t lower_y = 0;
        */
        uint16_t x_offset = 0;
        uint16_t size;

        uint16_t horz = 0;
        uint16_t vert = 0;
        unsigned char screen[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];

    public:
        text(cord_2d cord_1, cord_2d cord_2, colour colour_new, std::string text_to_render_new, const char *font_path, int font_size)
        {
            // Why on bloody earth does this work, I was using some funky loop to do it before
            /*{ // Port this over to whatever is being given before using this incase the std::copy thing doesnt work
                char text_2_add[] = "Hallo World";
                for (int i = 0; i < (int)strlen(text_2_add); i++)
                {
                    text_2_render.push_back(text_2_add[i]);
                    std::cout << text_2_render[i];
                }
            }*/
            std::cout << "Start text constructor thingy" << '\n';
            // std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_2_render));
            stbtt_InitFont(&font, file_buffer, 0);

            for (int i = 0; i < (int)strlen(text_2_render); i++)
            {
                std::cout << text_2_render[i];
            }
            std::cout << '\n';

            size = font_size;
            scale = stbtt_ScaleForPixelHeight(&font, 30);

            // Dont know what this does
            stbtt_GetFontVMetrics(&font, &ascent, 0, 0);

            // Note: Find the funky math to do the thing
            // do I just subtract the scale from the higher x?
            // God knows what this does
            baseline = (int)(ascent * scale);
            x_offset = 0;
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
                float x_shift = xpos - (float)floor(xpos) + 20;

                // Gets the data for this specific chacter
                stbtt_GetCodepointHMetrics(&font, text_2_render[ch], &advance, &lsb);

                // This tells us when the actual charcter starts & stops
                stbtt_GetCodepointBitmapBoxSubpixel(&font, text_2_render[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

                // Actually encode it?
                stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, LOGICAL_WINDOW_WIDTH, scale, scale, x_shift, 0, text_2_render[ch]);

                xpos += (advance * scale);
                if (text_2_render[ch + 1])
                {
                    xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
                }
                ch++;
            }

            ch = 0;
xpos = 2;
            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
                {
                    // Bit shift it a tad bit, and yeah
                    if ((int)screen[y_pos][x_pos] >> 5 > 0)
                    {
                        // Without the minus one, none of this works lol
                        render[x_pos][y_pos].r = ((int)screen[y_pos][x_pos] >> 5) * ((255 / 7) - 1);
                        render[x_pos][y_pos].g = ((int)screen[y_pos][x_pos] >> 5) * ((255 / 7) - 1);
                        render[x_pos][y_pos].b = ((int)screen[y_pos][x_pos] >> 5) * ((255 / 7) - 1);
                        render[x_pos][y_pos].a = ((int)screen[y_pos][x_pos] >> 5) * ((255 / 7) - 1);
                    }
                    // putchar(" .:ioVM@"[screen[x_pos][y_pos] >> 5]);
                }
                // putchar('\n');
            }
        }

        void change_colour(colour colour_new)
        {
            box_colour = colour_new;
        }

        ~text()
        {
            // free(screen);
        }
    };

}