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

        uint16_t higher_x = 0;
        uint16_t higher_y = 0;
        uint16_t lower_x = 0;
        uint16_t lower_y = 0;

        uint16_t horz = 0;
        uint16_t vert = 0;
        unsigned char *bitmap;


    public:
        text(cord_2d cord_1, cord_2d cord_2, colour colour_new, std::string text_to_render_new, const char *font_path, int font_size)
        {
            std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_2_render));
            // // perm = perm_new;
            // lower_x = std::min(cord_1.x_pos, cord_2.x_pos);
            // lower_y = std::min(cord_1.y_pos, cord_2.y_pos);
            // higher_x = std::max(cord_1.x_pos, cord_2.x_pos);
            // higher_y = std::max(cord_1.y_pos, cord_2.y_pos);
            // box_colour = colour_new;

            // horz = higher_x - lower_x;
            // vert = higher_y - lower_y;

            // // screen = (unsigned char *)malloc(sizeof(unsigned char) * (horz * vert));

            // std::cout << "Reading the file" << '\n';
            // fread(buffer, 1, 1000000, fopen(font_path, "rb"));

            // // We really should be giving the developer control
            // // Over these vars but ehhhhh
            // stbtt_InitFont(&font, buffer, 0);
            // // scale = stbtt_ScaleForPixelHeight(&font, 80);
            // // stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
            // // baseline = (int)(ascent * scale);

            // // God know what this does
            // // Go to `tex_test2.c, might help?
            // // c = ' ';

            // Refer to basic

        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
            /*
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
                            // ary[i*sizeY+j]
                            // (baseline + y0)+((int)xpos + x0)*horz
                            // baseline + y0
                            // (int)xpos + x0
                            stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, horz, scale, scale, x_shift, 0, text_2_render[ch]);

                            // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
                            // because this API is really for baking character bitmaps into textures. if you want to render
                            // a sequence of characters, you really need to render each bitmap to a temp buffer, then
                            // "alpha blend" that into the working buffer

                            // This is some recursive stuff, dont touch it OK
                            xpos += (advance * scale);
                            if (text_2_render[ch + 1])
                                xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
                            ++ch;
                        }

                        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
                        {
                            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
                            {
                                // if (lower_x < x_pos && x_pos < higher_x && lower_y < y_pos && y_pos < higher_y)
                                // {
                                int temp = screen[x_pos][y_pos] >> 5;
                                if (temp > 3)
                                {
                                    render[x_pos][y_pos].r = box_colour.r;
                                    render[x_pos][y_pos].g = box_colour.g;
                                    render[x_pos][y_pos].b = box_colour.b;
                                    render[x_pos][y_pos].a = box_colour.a;
                                }
                                // }
                            }
                        }
            */
            // can you read the docs before doing this
            // Go to the `a` example
            // also tex_test2.c
            // `h` : height of specific charcter
            // `w` : width of specific charcter
            // `j` & `i` : like x_pos & y_pos, but reversed

            /* This was sort of working just not well
                        cord_2d start;
                        cord_2d end;
                        end.x_pos = lower_x;
                        end.y_pos = lower_y;

                        bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), 'Z', &w, &h, 0, 0);
                        uint8_t height = h;
                        uint8_t width = w;


                        for (unsigned int index = 0; index < text_2_render.size(); index++)
                        {
                            bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), text_2_render[index], &w, &h, 0, 0);
                            start = end;
                            start.y_pos = lower_y;
                            end.x_pos = lower_x + (w * index);
                            end.y_pos = lower_y + height;

                            // for (j = 0; j < h; ++j)
                            // {
                            //     for (i = 0; i < w; ++i)
                            //     {
                            //         // putchar(" .:ioVM@"[bitmap[j * w + i] >> 5]);
                            //     }
                            //     // putchar('\n');
                            // }

                            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
                            {
                                for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
                                {
                                    // X_pos checks, then Y_pos checks
                                    if ((start.x_pos < x_pos && x_pos < end.x_pos) && (start.y_pos <= y_pos && y_pos < end.y_pos))
                                    {
                                        if ((bitmap[y_pos * w + x_pos] >> 5) > 3)
                                        {
                                            render[x_pos][y_pos].r = box_colour.r;
                                            render[x_pos][y_pos].g = box_colour.g;
                                            render[x_pos][y_pos].b = box_colour.b;
                                            render[x_pos][y_pos].a = box_colour.a;
                                        }
                                        else
                                        {
                                            render[x_pos][y_pos].r = 100;
                                            render[x_pos][y_pos].g = 100;
                                            render[x_pos][y_pos].b = 100;
                                            render[x_pos][y_pos].a = box_colour.a;
                                        }
                                    }
                                }
                                // std::cout << '\n';
                            }
                        }
                    }
            */
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