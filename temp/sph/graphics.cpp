#include "graphics.hpp"
#include <cmath>

uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];
int offsets[BUFFER_LINE * BUFFER_LINE];
int cells_ctr[BUFFER_LINE * BUFFER_LINE];
int particles_loc[FLOATER_AMT];

point* points = new point[POINTS_AMT];
floater* floatersA = new floater[FLOATER_AMT];
floater* floatersB = new floater[FLOATER_AMT];

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
