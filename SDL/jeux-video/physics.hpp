// #include "render.hpp"
// #include "base_data.hpp"

std::mutex mtx;

void sand_sim(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{

    // Sand is somewhat broken
    // Look into later, the comment later on has more details
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
        {


            // WORKS 
            if
            (
                pixels[x_pos][y_pos].state_now == solid &&
                pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty &&
                y_pos != (LOGICAL_WINDOW_WIDTH - 1)
            )
            {
                // std::cout << "Done" << std::endl;
                new_version[x_pos][y_pos + 1].r              = pixels[x_pos][y_pos].r          ;
                new_version[x_pos][y_pos + 1].g              = pixels[x_pos][y_pos].g          ;
                new_version[x_pos][y_pos + 1].b              = pixels[x_pos][y_pos].b          ;
                new_version[x_pos][y_pos + 1].a              = pixels[x_pos][y_pos].a          ;
                new_version[x_pos][y_pos + 1].state_now      = pixels[x_pos][y_pos].state_now  ;
                new_version[x_pos][y_pos + 1].temperature    = pixels[x_pos][y_pos].temperature;
                new_version[x_pos][y_pos + 1].pressure       = pixels[x_pos][y_pos].pressure   ;
                pixels[x_pos][y_pos].r           = 0    ;
                pixels[x_pos][y_pos].g           = 0    ;
                pixels[x_pos][y_pos].b           = 0    ;
                pixels[x_pos][y_pos].a           = 0    ;
                pixels[x_pos][y_pos].state_now   = empty;
                pixels[x_pos][y_pos].temperature = 0    ;
                pixels[x_pos][y_pos].pressure    = 0    ;
            }


            else if
            (
                pixels[x_pos][y_pos].state_now == solid &&
                pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty &&
                y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1
            )
            {
                // std::cout << "Done2" << std::endl;
                new_version[x_pos - 1][y_pos + 1].r              = pixels[x_pos][y_pos].r          ;
                new_version[x_pos - 1][y_pos + 1].g              = pixels[x_pos][y_pos].g          ;
                new_version[x_pos - 1][y_pos + 1].b              = pixels[x_pos][y_pos].b          ;
                new_version[x_pos - 1][y_pos + 1].a              = pixels[x_pos][y_pos].a          ;
                new_version[x_pos - 1][y_pos + 1].state_now      = pixels[x_pos][y_pos].state_now  ;
                new_version[x_pos - 1][y_pos + 1].temperature    = pixels[x_pos][y_pos].temperature;
                new_version[x_pos - 1][y_pos + 1].pressure       = pixels[x_pos][y_pos].pressure   ;
                pixels[x_pos][y_pos].r           = 0    ;
                pixels[x_pos][y_pos].g           = 0    ;
                pixels[x_pos][y_pos].b           = 0    ;
                pixels[x_pos][y_pos].a           = 0    ;
                pixels[x_pos][y_pos].state_now   = empty;
                pixels[x_pos][y_pos].temperature = 0    ;
                pixels[x_pos][y_pos].pressure    = 0    ;
            }

            // The issue is with the else statment and this, it's hard to fix lol
            // if
            // (
            //     pixels[x_pos][y_pos].state_now == solid &&
            //     new_version[x_pos + 1][y_pos + 1].state_now == empty && 
            //     pixels[x_pos + 1][y_pos + 1].state_now == empty
            //     && y_pos != 250 && x_pos != 250
            // )
            // {
            //     std::cout << "Done3" << std::endl;
            //     new_version[x_pos + 1][y_pos + 1].r              = pixels[x_pos][y_pos].r          ;
            //     new_version[x_pos + 1][y_pos + 1].g              = pixels[x_pos][y_pos].g          ;
            //     new_version[x_pos + 1][y_pos + 1].b              = pixels[x_pos][y_pos].b          ;
            //     new_version[x_pos + 1][y_pos + 1].a              = pixels[x_pos][y_pos].a          ;
            //     new_version[x_pos + 1][y_pos + 1].state_now      = pixels[x_pos][y_pos].state_now  ;
            //     new_version[x_pos + 1][y_pos + 1].temperature    = pixels[x_pos][y_pos].temperature;
            //     new_version[x_pos + 1][y_pos + 1].pressure       = pixels[x_pos][y_pos].pressure   ;

            // }



            else
            {
                new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
            }

        }
    }
    // mtx.lock();
    // memcpy(&pixels, &new_version, sizeof(pixels));
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            pixels[x_pos][y_pos] =  new_version[x_pos][y_pos];
            new_version[x_pos][y_pos].r           = 0    ;
            new_version[x_pos][y_pos].g           = 0    ;
            new_version[x_pos][y_pos].b           = 0    ;
            new_version[x_pos][y_pos].a           = 0    ;
            new_version[x_pos][y_pos].state_now   = empty;
            new_version[x_pos][y_pos].temperature = 0    ;
            new_version[x_pos][y_pos].pressure    = 0    ;
        }
    }
    // mtx.unlock();

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

void simulate(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], bool quit)
{

    while(!quit)
    {
        auto start_time = Clock::now();

        sand_sim(pixels, new_version);

        auto end_time = Clock::now();

        if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 22222)
        {
            SDL_Delay((22222 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 100000);
        }

    }

    std::cout << "It don't matter it done" << std::endl;
}