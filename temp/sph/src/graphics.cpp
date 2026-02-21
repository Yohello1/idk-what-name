#include "floaters.hpp"
#include "graphics.hpp"
#include "spatial.hpp"
#include "math.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

namespace JD::graphics {

    uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];
    int offsets[BUFFER_LINE * BUFFER_LINE];
    int cells_ctr[BUFFER_LINE * BUFFER_LINE];
    int particles_loc[FLOATER_AMT];

    // TODO make this a calculation based function lol
    point* points = new point[POINTS_AMT];

    void draw_line_std_pair(uint8_t* buffer, std::pair<int, int> p0, std::pair<int, int> p1,
                            uint8_t r, uint8_t g, uint8_t b) {
        int x0 = p0.first, y0 = p0.second;
        int x1 = p1.first, y1 = p1.second;
        int dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;

        while (true) {
            if (x0 >= 0 && x0 < BUFFER_WIDTH && y0 >= 0 && y0 < BUFFER_HEIGHT) {
                int index = (y0 * BUFFER_WIDTH + x0) * BYTES_PER_PIXEL;
                static_rgb_buffer[index + 0] = r;
                static_rgb_buffer[index + 1] = g;
                static_rgb_buffer[index + 2] = b;
            }
            if (x0 == x1 && y0 == y1) break;
            e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 < dy) { err += dx; y0 += sy; }
        }
    }

    void initGrid() {
        std::vector<std::pair<int, int>> derivative = JD::spatial::calculateRegionsOffsets(); 
        std::vector<int> relative_offsets {};

        for(int i = 0; i < REGIONS_AMT; i++) {
            relative_offsets.push_back(derivative[i].first + (derivative[i].second * BUFFER_LINE));
        }

        std::sort(relative_offsets.begin(), relative_offsets.end());

        for(int i = 0; i < POINTS_AMT; i++) {
            int x = (i % POINTS_WIDTH);
            int y = (i / POINTS_WIDTH);

            points[i].x = x;
            points[i].y = y;
            points[i].i_x = x * DISTANCE_BETWEEN_POINTS + BUFFER_PADDING;
            points[i].i_y = y * DISTANCE_BETWEEN_POINTS + BUFFER_PADDING;
            points[i].id = i;
            points[i].strength = rand() % 256;

            int cell_x = points[i].i_x / DISTANCE_BETWEEN_POINTS;
            int cell_y = points[i].i_y / DISTANCE_BETWEEN_POINTS;
            int base_cell_id = cell_x + BUFFER_LINE * cell_y;

            for(int j = 0; j < REGIONS_AMT; j++) {
                points[i].regions[j] = base_cell_id + relative_offsets[j];
            }
        }
    }

    void InitializeStaticBuffer() {
        for (int y = 0; y < BUFFER_HEIGHT; ++y) {
            for (int x = 0; x < BUFFER_WIDTH; ++x) {
                int buffer_index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;
                static_rgb_buffer[buffer_index + 0] = 0; // Red
                static_rgb_buffer[buffer_index + 1] = 0; // Green
                static_rgb_buffer[buffer_index + 2] = 0; // Blue
            }
        }
    }

    void computeStrengths() {
        floater* __restrict p_floatersA = JD::floaters::floatersA;
        int* __restrict p_indices = particles_loc;

        for(int i = 0; i < POINTS_AMT; i++) {
            float strength = 0.0f;
            int x = points[i].i_x;
            int y = points[i].i_y;

            for(int j = 0; j < REGIONS_AMT; j++) {
                int idx_r = points[i].regions[j];
                int idx_o = offsets[idx_r];

                for(int k = 0; k < cells_ctr[idx_r]; k++) {
                    int floater_idx = p_indices[idx_o + k];
                    float dx = p_floatersA[floater_idx].x - x;
                    float dy = p_floatersA[floater_idx].y - y;
                    float dist_sq = dx*dx + dy*dy;

                    dist_sq = (dist_sq < 0.001f) ? 0.001f : dist_sq;
                    strength += (p_floatersA[floater_idx].density) * (1.0f / dist_sq);
                }
            }
            points[i].strength = (uint16_t)strength;
        }
    }

    void drawConnections() {
        const int SQR_PER_ROW = POINTS_WIDTH - 1;
        const int SQR_PER_COL = POINTS_HEIGHT - 1;

        static const int8_t LUT[16][4] = {
            {-1,-1,-1,-1}, {0,3,-1,-1}, {0,1,-1,-1}, {3,1,-1,-1},
            {1,2,-1,-1},   {0,1,2,3},   {0,2,-1,-1}, {3,2,-1,-1},
            {3,2,-1,-1},   {0,2,-1,-1}, {0,3,1,2},   {1,2,-1,-1},
            {3,1,-1,-1},   {0,1,-1,-1}, {0,3,-1,-1}, {-1,-1,-1,-1}
        };

        const int R = 255, G = 255, B = 255;

        for(int i = 0; i < SQR_PER_ROW; i++) {
            int row_top = i * POINTS_WIDTH;
            int row_bot = (i + 1) * POINTS_WIDTH;

            for(int j = 0; j < SQR_PER_COL; j++) {
                int config_index = (points[row_top + j].strength     >= THRESHOLD)      |
                ((points[row_top + j + 1].strength >= THRESHOLD) << 1) |
                ((points[row_bot + j + 1].strength >= THRESHOLD) << 2) |
                ((points[row_bot + j].strength     >= THRESHOLD) << 3);

                if (config_index == 0 || config_index == 15) continue;

                std::pair<int, int> pts[4];
                pts[0] = JD::math::getMidPoint(points[row_top + j],     points[row_top + j + 1]);
                pts[1] = JD::math::getMidPoint(points[row_top + j + 1], points[row_bot + j + 1]);
                pts[2] = JD::math::getMidPoint(points[row_bot + j + 1], points[row_bot + j]);
                pts[3] = JD::math::getMidPoint(points[row_bot + j],     points[row_top + j]);

                const int8_t* edges = LUT[config_index];
                draw_line_std_pair(static_rgb_buffer, pts[edges[0]], pts[edges[1]], R, G, B);

                if (edges[2] != -1) {
                    draw_line_std_pair(static_rgb_buffer, pts[edges[2]], pts[edges[3]], R, G, B);
                }
            }
        }
    }

    void drawGrid() {
        for(int i = 0; i < POINTS_AMT; i++) {
            int idx = points[i].i_x * BYTES_PER_PIXEL + points[i].i_y * BUFFER_WIDTH * BYTES_PER_PIXEL;
            if(points[i].strength >= THRESHOLD) {
                static_rgb_buffer[idx] = 255;
                static_rgb_buffer[idx+1] = 255;
                static_rgb_buffer[idx+2] = 255;
            }
        }
    }

} // namespace JD::graphics
