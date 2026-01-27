#include "floaters.hpp"
#include "graphics.hpp"

namespace JD::floaters
{
    void initFloaters()
    {
        for(int i = 0; i < FLOATER_AMT; i++)
        {
            JD::graphics::floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
            JD::graphics::floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;

            JD::graphics::floatersA[i].v_x = ((rand() % 50) - 25)/10;
            JD::graphics::floatersA[i].v_y = ((rand() % 50) - 25)/10;

            JD::graphics::floatersA[i].mass = 50;

            JD::graphics::floatersA[i].density = PARTICLE_REFERENCE_DENSITY;

            // std::cout << floatersA[i].density << ' ';
        }
        // std::cout << '\n';
    }


    void drawFloaters()
    {
        for(int i = 0; i < FLOATER_AMT; i++)
        {
            int idx = JD::graphics::floatersA[i].x*BYTES_PER_PIXEL + JD::graphics::floatersA[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
            JD::graphics::static_rgb_buffer[idx+1]   = 250;
        }
    }
}
