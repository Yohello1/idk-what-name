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
    void draw_ui()
    {
        // wut
        // I am not smort
    }

    // Cant I just copy this fron sandy thing but with a diff set of stuff???
    // void draw_box(cord_2d start, cord_2d end, colour colours, colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    // {
    //     // The location is relative to where it is on the screen/window :P
    //     // That made no sense
    //     int higher_x = std::min(start.x_pos, end.x_pos);
    //     int higher_y = std::min(start.y_pos, end.y_pos);
    //     int lower_x = std::max(start.x_pos, end.y_pos);
    //     int lower_y = std::max(start.y_pos, end.y_pos);

    //     for (int x_pos = higher_x; x_pos < lower_x; x_pos++)
    //     {
    //         for (int y_pos = higher_y; y_pos < lower_y; y_pos++)
    //         {
    //             user_interface[x_pos][y_pos].r = colours.r;
    //             user_interface[x_pos][y_pos].g = colours.g;
    //             user_interface[x_pos][y_pos].b = colours.b;
    //             user_interface[x_pos][y_pos].a = colours.a;
    //             // std::cout << "Hello world" << '\n';
    //         }
    //     }
    // }
    // not even legacy
    // void merge_render_and_user_interface(colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    // {
    //     for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    //     {
    //         for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    //         {
    //             render[x_pos][y_pos].r = (user_interface[x_pos][y_pos].r * user_interface[x_pos][y_pos].a + render[x_pos][y_pos].r) / (1 + (user_interface[x_pos][y_pos].a / 255));
    //             render[x_pos][y_pos].g = (user_interface[x_pos][y_p  os].g * user_interface[x_pos][y_pos].a + render[x_pos][y_pos].g) / (1 + (user_interface[x_pos][y_pos].a / 255));
    //             render[x_pos][y_pos].b = (user_interface[x_pos][y_pos].b * user_interface[x_pos][y_pos].a + render[x_pos][y_pos].b) / (1 + (user_interface[x_pos][y_pos].a / 255));
    //             render[x_pos][y_pos].a = 255;
    //             // std::cout << "hi" << '\n';
    //         }
    //     }
    // }


    class ui_element
    {
        virtual ~ui_element();
    };


    class boxes : public ui_element
    {
    private:
        // we're not gonna deal with permanent things yet
        // bool perm = false;
        colour box_colour;

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
            higher_x  = std::max(cord_1.x_pos, cord_2.x_pos);
            higher_y  = std::max(cord_1.y_pos, cord_2.y_pos);
            box_colour = colour_new;
        }

        void draw_box(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
            for(int x_pos = lower_x; x_pos < higher_x; x_pos++)
            {
                for(int y_pos = lower_y; y_pos < higher_y; y_pos++)
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

}