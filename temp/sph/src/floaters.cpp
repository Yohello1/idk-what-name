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
        // Everything gets init random,
        // then afterwards we select the subset we need for the outside ya
        // from end
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            floatersA[i].x = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;
            floatersA[i].y = (rand() % BUFFER_WORKING-1)+BUFFER_PADDING;

            floatersA[i].v_x = ((rand() % 50) - 25)/10;
            floatersA[i].v_y = ((rand() % 50) - 25)/10;

            floatersA[i].mass = 50;
            floatersA[i].density = PARTICLE_REFERENCE_DENSITY;
            floatersA[i].enabled = true;
        }


        size_t current_i = DESIRED_FLOATERS;
        int shell_idx = 0;
        while (current_i < FLOATER_AMT) {
            int x0 = BUFFER_PADDING - shell_idx;
            int y0 = BUFFER_PADDING - shell_idx;
            int x1 = BUFFER_PADDING + BUFFER_WORKING + shell_idx;
            int y1 = BUFFER_PADDING + BUFFER_WORKING + shell_idx;

            // Top edge
            for (int x = x0; x < x1 && current_i < FLOATER_AMT; x++) {
                floatersA[current_i].x = (float)x;
                floatersA[current_i].y = (float)y0;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Right edge
            for (int y = y0; y < y1 && current_i < FLOATER_AMT; y++) {
                floatersA[current_i].x = (float)x1;
                floatersA[current_i].y = (float)y;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Bottom edge
            for (int x = x1; x > x0 && current_i < FLOATER_AMT; x--) {
                floatersA[current_i].x = (float)x;
                floatersA[current_i].y = (float)y1;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            // Left edge
            for (int y = y1; y > y0 && current_i < FLOATER_AMT; y--) {
                floatersA[current_i].x = (float)x0;
                floatersA[current_i].y = (float)y;
                floatersA[current_i].v_x = 0;
                floatersA[current_i].v_y = 0;
                floatersA[current_i].enabled = false;
                current_i++;
            }
            shell_idx++;
            if (shell_idx > BUFFER_PADDING) break; 
        }
    
    }


    void drawFloaters()
    {
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            int idx = floatersA[i].x*BYTES_PER_PIXEL + floatersA[i].y*BUFFER_WIDTH*BYTES_PER_PIXEL;
            JD::graphics::static_rgb_buffer[idx+1]   = 250;
        }
    }

    // I used an ai to generate this, so if smth breaks
    // rewrite this first
    // before going anywhere else
    // oki
    __attribute__ ((noinline))
    void initBlockRegions() {
        int W = (int) BUFFER_LINE; // Assuming BUFFER_LINE is your grid width

        for(size_t i = 0; i < BLOCK_AMT; i++) {
            int row = i / W;
            int col = i % W;
            int count = 0;

            // Iterate through the 3x3 neighborhood
            for(int dy = -1; dy <= 1; dy++) {
                for(int dx = -1; dx <= 1; dx++) {
                    int targetRow = row + dy;
                    int targetCol = col + dx;
                    int targetIndex = i + (dy * W) + dx;

                    // Bounds checking:
                    // 1. Is the row valid?
                    // 2. Is the column valid? (Prevents horizontal wrapping)
                    // 3. Is the resulting index within the array?
                    if (targetRow >= 0 && targetRow < (BLOCK_AMT / W) &&
                        targetCol >= 0 && targetCol < W &&
                        targetIndex >= 0 && targetIndex < BLOCK_AMT) {
                        blocks[i].regions[count++] = targetIndex;
                    } else {
                        blocks[i].regions[count++] = INT_MAX;
                    }
                }
            }
        }
    }
}
