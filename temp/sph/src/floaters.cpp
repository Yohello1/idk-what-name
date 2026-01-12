
#include "floaters.hpp"
#include "graphics.hpp"


void initFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
        floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING; 

        floatersA[i].v_x = ((rand() % 50) - 25)/10;
        floatersA[i].v_y = ((rand() % 50) - 25)/10;

        floatersA[i].r = (rand() % 127)+128;
        floatersA[i].b = (rand() % 127)+128;

        floatersA[i].density = PARTICLE_REFERENCE_DENSITY;
    
        // std::cout << floatersA[i].density << ' ';
    }
    // std::cout << '\n';
}

void simulateFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        floatersA[i].x += floatersA[i].v_x;
        floatersA[i].y += floatersA[i].v_y;


        if (floatersA[i].x < (float) BUFFER_PADDING)
        {
            floatersA[i].v_x *= -1.0f;
            floatersA[i].x = BUFFER_PADDING;
        }


        else if (floatersA[i].x > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floatersA[i].v_x *= -1.0f;
            floatersA[i].x = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }


        if (floatersA[i].y < (float) BUFFER_PADDING)
        {
            floatersA[i].v_y *= -1.0f;
            floatersA[i].y = BUFFER_PADDING;
        }

        else if (floatersA[i].y > (float) (BUFFER_WORKING + BUFFER_PADDING-1))
        {
            floatersA[i].v_y *= -1.0f;
            floatersA[i].y = (BUFFER_WORKING + BUFFER_PADDING - 1);
        }
    }
}

void drawFloaters()
{
    for(int i = 0; i < FLOATER_AMT; i++)
    {
        int idx = floatersA[i].x*BYTES_PER_PIXEL + floatersA[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
        static_rgb_buffer[idx]   = floatersA[i].r;
        static_rgb_buffer[idx+1] = floatersA[i].b;
    }
}

