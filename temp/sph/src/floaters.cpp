#include "floaters.hpp"
#include "graphics.hpp"
#include "struct.hpp"

namespace JD::floaters
{

    floater* floatersA = new floater[ FLOATER_AMT ];
    floater* floatersB = new floater[ FLOATER_AMT ];
    
    void initFloaters()
    {
        for(int i = 0; i < FLOATER_AMT; i++)
        {
            floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
            floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;

            floatersA[i].v_x = ((rand() % 50) - 25)/10;
            floatersA[i].v_y = ((rand() % 50) - 25)/10;

            floatersA[i].mass = 50;

            floatersA[i].density = PARTICLE_REFERENCE_DENSITY;

            // std::cout << floatersA[i].density << ' ';
        }
        // std::cout << '\n';
    }


    void drawFloaters()
    {
        for(int i = 0; i < FLOATER_AMT; i++)
        {
            int idx = floatersA[i].x*BYTES_PER_PIXEL + floatersA[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
            JD::graphics::static_rgb_buffer[idx+1]   = 250;
        }
    }
}
