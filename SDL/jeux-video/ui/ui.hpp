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
    int i, j, ascent, ch = 0;

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
            std::cout << "hi";
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
        std::vector<char> text_to_render;

        int higher_x = 0;
        int higher_y = 0;
        int lower_x = 0;
        int lower_y = 0;

    public:
        text(cord_2d cord_1, cord_2d cord_2, colour colour_new, std::string text_to_render_new, const char *font_path)
        {
            std::copy(text_to_render_new.begin(), text_to_render_new.end(), std::back_inserter(text_to_render));
            // perm = perm_new;
            lower_x = std::min(cord_1.x_pos, cord_2.x_pos);
            lower_y = std::min(cord_1.y_pos, cord_2.y_pos);
            higher_x = std::max(cord_1.x_pos, cord_2.x_pos);
            higher_y = std::max(cord_1.y_pos, cord_2.y_pos);
            box_colour = colour_new;

            std::cout << "Reading the file" << '\n';
            fread(buffer, 1, 1000000, fopen(font_path, "rb"));
            stbtt_InitFont(&font, buffer, 0);
            scale = stbtt_ScaleForPixelHeight(&font, 15);
        }

        void draw(position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
        {
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