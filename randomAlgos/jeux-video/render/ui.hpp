// #include <..base_data.hpp>

/*
Oi listen up
Me from 2 y/o was not listening :P

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
    unsigned char file_buffer[24 << 20]; // this should rlly be in a class to allow multiple files but thats a future problem

    // Init fonts
    void init_font_all(const char *font_path)
    {
        // Puts it into the file buffer
        // IT CANNOT POSSIBLY BE THAT BIG
        fread(file_buffer, 1, 1000000, fopen(font_path, "rb"));
        stbtt_InitFont(&font, file_buffer, 0);
    }


    // Basic class, everything is based off of this
    class single_ui_element
    {
        public:
            cord_2d size;
            colour_t *renderResult;
            virtual ~single_ui_element() = default;
            virtual void* draw()
            { std::cout << "Wrong function, you called draw on the parent class of the ui elements" << '\n'; }
        protected:
            colour_t boxColour;
    };


    // Basic boxes
    class boxes : public single_ui_element
    {
        private:
            cord_2d lower;
            cord_2d higher;
        public:
            // size of map, cord 1, cord 2, colour
            boxes(cord_2d sizeIn, cord_2d cord_1, cord_2d cord_2, colour_t colour_new)
            {
                lower.x_pos  = std::min(cord_1.x_pos, cord_2.x_pos);
                lower.y_pos  = std::min(cord_1.y_pos, cord_2.y_pos);
                higher.x_pos = std::max(cord_1.x_pos, cord_2.x_pos);
                higher.y_pos = std::max(cord_1.y_pos, cord_2.y_pos);
                boxColour = colour_new;
                size.x_pos = sizeIn.x_pos;
                size.y_pos = sizeIn.y_pos;
                renderResult = new colour_t[sizeIn.x_pos * sizeIn.y_pos];
            }

            // This will return a point to a 2d array of size in the member called size
            // which has the stuff, access that point to access the map of stuff
            void * draw()
            {
                // std::cout << "bagguette" << '\n';
                for (int x_pos = lower.x_pos; x_pos < higher.x_pos; x_pos++)
                {
                    for (int y_pos = lower.y_pos; y_pos < higher.y_pos; y_pos++)
                    {
                        if((y_pos*size.y_pos + x_pos) > 0 && (size.x_pos*4*size.y_pos) < ((y_pos*size.x_pos + x_pos*4)))
                        {
                            renderResult[(y_pos*size.x_pos + x_pos)].r = boxColour.r; // red
                            renderResult[(y_pos*size.x_pos + x_pos)].g = boxColour.g;// green
                            renderResult[(y_pos*size.x_pos + x_pos)].b = boxColour.b; // blue
                            renderResult[(y_pos*size.x_pos + x_pos)].a = boxColour.a; // alpha
                        }
                    }
                }
                return renderResult;
            }

            void change_colour(colour_t colourNew)
            {
                boxColour = colourNew;
            }
    };


    // Text rendering class
    class text : public single_ui_element
    {
        private:

            // unsigned char screen[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH]; // in thory this can be used for resolution

        public:
            // Put a space at the begining of the string, idk why but if you dont it breaks everything
            // like " hello" instead of "hello"
            text()
            {
                // // Why on bloody earth does this work
                // // Copies the text from the string into a vector/array
                // std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_2_render));

                // // Sets the colour, and location
                // txt_color = colour_new;
                // location = location_in;

                // // Size
                // scale = stbtt_ScaleForPixelHeight(&font, font_size);

                // // No clue
                // stbtt_GetFontVMetrics(&font, &ascent, 0, 0);

                // // Note: Find the funky math to do the thing
                // // do I just subtract the scale from the higher x?
                // // God knows what this does
                // baseline = (int)(ascent * scale) + location.y_pos;
                // std::cout << (int)location.x_pos << '\n';
                // min_power = min_power_in;
                // std::cout << "End text constructor thingy" << '\n';
            }

            colour_t* draw()
            {

//                 while (text_2_render[ch])
//                 {
//                     // Get the cords for evetyhing
//                     int advance, lsb, x0, y0, x1, y1;

//                     // not a single clue what this is supposed to do lol
//                     // add to move it right
//                     float x_shift = xpos - (float)floor(xpos) + location.x_pos;

//                     // Gets the data for this specific chacter
//                     stbtt_GetCodepointHMetrics(&font, text_2_render[ch], &advance, &lsb);

//                     // This tells us when the actual charcter starts & stops
//                     stbtt_GetCodepointBitmapBoxSubpixel(&font, text_2_render[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

//                     // Actually encode it?
//                     stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, LOGICAL_WINDOW_WIDTH, scale, scale, x_shift, 0, text_2_render[ch]);

//                     xpos += (advance * scale);
//                     if (text_2_render[ch + 1])
//                     {
//                         xpos += scale * stbtt_GetCodepointKernAdvance(&font, text_2_render[ch], text_2_render[ch + 1]);
//                     }
//                     ch++;
//                 }

//                 ch = 0;
//                 xpos = 2;

//                 // I can def speed this up
//                 // Doing the same sort of thing i do for input drawing would work
//                 // What did I mean by that???
//                 for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
//                 {
//                     for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
//                     {
//                         // Bit shift it a tad bit, and yeah
//                         if ((int)screen[y_pos][x_pos] >> 5 > min_power)
//                         {
// #if BLEND_TEXT == 0
//                             // render[x_pos][y_pos].change_r(((render[x_pos][y_pos].fetch_r() * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.r / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5))));
//                             // render[x_pos][y_pos].change_g(((render[x_pos][y_pos].fetch_g() * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.g / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5))));
//                             // render[x_pos][y_pos].change_b(((render[x_pos][y_pos].fetch_b() * (7 - (screen[y_pos][x_pos] >> 5)) + (txt_color.b / 7) * (screen[y_pos][x_pos] >> 5)) / (7 - 0 - (screen[y_pos][x_pos] >> 5) + (screen[y_pos][x_pos] >> 5) / (screen[y_pos][x_pos] >> 5))));
//                             // render[x_pos][y_pos].change_a(255);
// #elif BLEND_TEXT == 1
//                             render[x_pos][y_pos].change_r(txt_color.r);
//                             render[x_pos][y_pos].change_g(txt_color.g);
//                             render[x_pos][y_pos].change_b(txt_color.b);
//                             render[x_pos][y_pos].change_a(255);
// #endif

//                         }
//                     }
//                 }
            }

            void change_colour(colour_t colour_new)
            {
                // box_colour = colour_new;
            }

            ~text()
            {
                // free(screen);
            }
    };

}
