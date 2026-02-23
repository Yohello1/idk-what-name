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
            int gx = (JD::floaters::floatersA[i].x) / DISTANCE_BETWEEN_POINTS;
            int gy = (JD::floaters::floatersA[i].y) / DISTANCE_BETWEEN_POINTS;

            if (gx >= 0 && gx < BUFFER_LINE && gy >= 0 && gy < BUFFER_LINE) {
                int idx = gx + gy * BUFFER_LINE;
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

    uint32_t* _curr_pos = (uint32_t*)calloc(sizeof(uint32_t), BUFFER_LINE*BUFFER_LINE);
    void computeIndicies()
    {
        std::memset(_curr_pos, 0, sizeof(uint32_t)*BUFFER_LINE*BUFFER_LINE);
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            int gx = (JD::floaters::floatersA[i].x) / DISTANCE_BETWEEN_POINTS;
            int gy = (JD::floaters::floatersA[i].y) / DISTANCE_BETWEEN_POINTS;

            if (gx >= 0 && gx < BUFFER_LINE && gy >= 0 && gy < BUFFER_LINE) {
                int idx = gx + gy * BUFFER_LINE;
                int offset = JD::graphics::offsets[idx];
                /// this should never happen................
                /// (more thingies than expected)
                if ((size_t)_curr_pos[idx] < (size_t)JD::graphics::cells_ctr[idx]) {
                    JD::graphics::particles_loc[offset + _curr_pos[idx]] = i;
                    _curr_pos[idx] += 1;
                }
            }
        }
    }

}
