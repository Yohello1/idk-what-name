// #include "render.hpp"
// #include "base_data.hpp"

std::mutex mtx;

void sand_sim(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
        {
            if (pixels[x_pos][y_pos].state_now == solid && y_pos != (LOGICAL_WINDOW_WIDTH-1) && pixels[x_pos][y_pos + 1].state_now == empty)
            {

                std::cout << "Valid move" << std::endl;
                new_version[x_pos][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos][y_pos].r = pixels[x_pos][y_pos].g = new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].a = 0;
                new_version[x_pos][y_pos].state_now = empty;
            }
            else if (pixels[x_pos][y_pos].state_now == solid && y_pos != (LOGICAL_WINDOW_WIDTH-1) && pixels[x_pos + 1][y_pos + 1].state_now == empty)
            {
                // std::cout << "Valid move" << std::endl;
                new_version[x_pos + 1][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos][y_pos].r = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].a = 0;
                new_version[x_pos][y_pos].state_now = empty;
            }
            else if (pixels[x_pos][y_pos].state_now == solid && y_pos != (LOGICAL_WINDOW_WIDTH-1) && pixels[x_pos - 1][y_pos + 1].state_now == empty)
            {
                 std::cout << "Valid move" << std::endl;
                new_version[x_pos - 1][y_pos + 1] = pixels[x_pos][y_pos];
                new_version[x_pos][y_pos].r = new_version[x_pos][y_pos].g = new_version[x_pos][y_pos].b = new_version[x_pos][y_pos].a = 0;
                new_version[x_pos][y_pos].state_now = empty;
            }
            else
            {
                new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
            }

        }
    }
    mtx.lock();
    // memcpy(&pixels, &new_version, sizeof(pixels));
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
        {
            pixels[x_pos][y_pos] =  new_version[x_pos][y_pos];
        }
    }
    mtx.unlock();

}

void draw_box_white_sand(cord_2d start, cord_2d end, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    // The location is relative to where it is on the screen/window :P
    int higher_x = std::min(start.x_pos, end.x_pos);
    int higher_y = std::min(start.y_pos, end.y_pos);
    int  lower_x = std::max(start.x_pos, end.y_pos);
    int  lower_y = std::max(start.y_pos, end.y_pos);

    for(int x_pos = higher_x; x_pos < lower_x; x_pos++)
    {
        for(int y_pos = higher_y; y_pos < lower_y; y_pos++)
        {
            pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 255;
            pixels[x_pos][y_pos].state_now = solid;
        }
    }

}

void simulate_single(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    sand_sim(pixels, new_version);
}