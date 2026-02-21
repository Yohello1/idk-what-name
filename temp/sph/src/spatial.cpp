#include "spatial.hpp"
#include "floaters.hpp"

#include "settings.hpp"
#include "struct.hpp"
#include "graphics.hpp"
#include <vector>
#include <cstring>

namespace JD::spatial
{
    void offsetsCreation()
    {
        memset(JD::graphics::cells_ctr, 0, sizeof(int)*(BUFFER_LINE*BUFFER_LINE));

        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            // removed - BUFFER_PADDING
            // because it makes no sense to be removing it?
            int gx = (JD::floaters::floatersA[i].x) / DISTANCE_BETWEEN_POINTS;
            int gy = (JD::floaters::floatersA[i].y) / DISTANCE_BETWEEN_POINTS;

            int idx = (gx) + (gy) * BUFFER_LINE;

            if (idx >= 0 && idx < (BUFFER_LINE * BUFFER_LINE)) {
                JD::graphics::cells_ctr[idx] += 1;
            }
        }

        for(int i = 0, j = 0; i < (BUFFER_LINE * BUFFER_LINE); i++)
        {
            JD::graphics::offsets[i] = j;
            j += JD::graphics::cells_ctr[i];
        }
    }

    std::vector<std::pair<int, int>> calculateRegionsOffsets()
    {
        std::vector<std::pair<int, int>> combined_list;

        for (int y = -INFLUENCE_RADIUS; y <= INFLUENCE_RADIUS; ++y) {
            for (int x = -INFLUENCE_RADIUS; x <= INFLUENCE_RADIUS; ++x) {
                if (std::abs(x) + std::abs(y) <= INFLUENCE_RADIUS) {
                    combined_list.push_back({x, y});
                }
            }
        }

        return combined_list;
    }

    void computeIndicies()
    {
        uint32_t* curr_pos = (uint32_t*)calloc(sizeof(uint32_t), BUFFER_LINE * BUFFER_LINE);

        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            int gx = (JD::floaters::floatersA[i].x - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;
            int gy = (JD::floaters::floatersA[i].y - BUFFER_PADDING) / DISTANCE_BETWEEN_POINTS;

            int idx = (gx + PADDING) + (gy + PADDING) * BUFFER_LINE;

            if (idx >= 0 && idx < (BUFFER_LINE * BUFFER_LINE)) {
                int offset = JD::graphics::offsets[idx];
                JD::graphics::particles_loc[offset + curr_pos[idx]] = i;
                curr_pos[idx] += 1;
            }
        }
        free(curr_pos);
    }

}
