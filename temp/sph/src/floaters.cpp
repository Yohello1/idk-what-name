#include <iostream>

#include "floaters.hpp"
#include "graphics.hpp"
#include "struct.hpp"

namespace JD::floaters
{

    floater* floatersA = new floater[ FLOATER_AMT ];
    block*   blocks    = new block  [ BLOCK_AMT   ];


    void init()
    {
        initFloaters();
        initBlockRegions();
    }

    void initFloaters()
    {
        // initialize EVERYTHING to disabled out-of-bounds first
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            floatersA[i].x = -1000.0f;
            floatersA[i].y = -1000.0f;
            floatersA[i].v_x = 0;
            floatersA[i].v_y = 0;
            floatersA[i].mass = PARTICLE_MASS;
            floatersA[i].density = PARTICLE_REFERENCE_DENSITY;
            floatersA[i].enabled = false;
        }

        // only enable and spawn the desired working fluid
        for(size_t i = 0; i < DESIRED_FLOATERS; i++)
        {
            floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
            floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;

            floatersA[i].v_x = ((rand() % 50) - 25)/10;
            floatersA[i].v_y = ((rand() % 50) - 25)/10;

            floatersA[i].enabled = true;
        }


        size_t current_i = DESIRED_FLOATERS;
        int shell_idx = 0;
        while (current_i < FLOATER_AMT) {
            int x0 = BUFFER_PADDING - shell_idx;
            int y0 = BUFFER_PADDING - shell_idx;
            int x1 = BUFFER_PADDING + BUFFER_WORKING + shell_idx;
            int y1 = BUFFER_PADDING + BUFFER_WORKING + shell_idx;

            std::cout << current_i << ' ';

            // Top edge
            for (int x = x0; x < x1 && current_i < FLOATER_AMT; x += PARTICLE_GHOST_DENSITY) {
                floatersA[current_i].x = (float)x;
                floatersA[current_i].y = (float)y0;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Right edge
            for (int y = y0; y < y1 && current_i < FLOATER_AMT; y += PARTICLE_GHOST_DENSITY) {
                floatersA[current_i].x = (float)x1;
                floatersA[current_i].y = (float)y;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Bottom edge
            for (int x = x1; x > x0 && current_i < FLOATER_AMT; x -= PARTICLE_GHOST_DENSITY) {
                floatersA[current_i].x = (float)x;
                floatersA[current_i].y = (float)y1;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Left edge
            for (int y = y1; y > y0 && current_i < FLOATER_AMT; y -= PARTICLE_GHOST_DENSITY) {
                floatersA[current_i].x = (float)x0;
                floatersA[current_i].y = (float)y;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            shell_idx += PARTICLE_GHOST_DENSITY;
            if (x0 <= 0 || y0 <= 0) break; 
        }
    
    }


    void drawFloaters()
    {
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            int px = (int)floatersA[i].x;
            int py = (int)floatersA[i].y;
            
            if (px >= 0 && px < BUFFER_WIDTH && py >= 0 && py < BUFFER_HEIGHT) {
                int idx = px * BYTES_PER_PIXEL + py * BUFFER_WIDTH * BYTES_PER_PIXEL;
                JD::graphics::static_rgb_buffer[idx+1] = 250;
            }
        }
    }

    // each block stores flat incides of all grid cells within influence_radius
    // in axis directions (2*IR+1)^2
    // must match kernel radii so ghost aprticles placed just outside region
    // are visible to fluid particles at
    // border
    //
    __attribute__ ((noinline))
    void initBlockRegions() {
        const int W     = (int)BUFFER_LINE;
        const int H     = (int)(BLOCK_AMT / W);
        const int IR    = INFLUENCE_RADIUS;
        const int COUNT = BLOCK_NEIGHBOR_COUNT;

        for (size_t i = 0; i < BLOCK_AMT; i++) {
            int row   = (int)(i / W);
            int col   = (int)(i % W);
            int count = 0;

            for (int dy = -IR; dy <= IR; dy++) {
                for (int dx = -IR; dx <= IR; dx++) {
                    int targetRow   = row + dy;
                    int targetCol   = col + dx;
                    int targetIndex = (int)i + dy * W + dx;

                    if (targetRow   >= 0 && targetRow   < H  &&
                        targetCol   >= 0 && targetCol   < W  &&
                        targetIndex >= 0 && targetIndex < (int)BLOCK_AMT) {
                        blocks[i].regions[count++] = (uint32_t)targetIndex;
                    } else {
                        blocks[i].regions[count++] = (uint32_t)INT_MAX;
                    }
                }
            }
            // int_max for everything else
            // this is for people who are bad at coding
            // (I am bad at coding)
            while (count < COUNT) blocks[i].regions[count++] = (uint32_t)INT_MAX;
        }
    }
}
