#ifndef _GRAPHICS_CPP
#define _GRAPHICS_CPP

#include "graphics.hpp"
#include "settings.hpp"

void draw_line_std_pair(uint8_t* buffer,
                        std::pair<int, int> p0,
                        std::pair<int, int> p1,
                        uint8_t r, uint8_t g, uint8_t b) {

    int x0 = p0.first;
    int y0 = p0.second;
    int x1 = p1.first;
    int y1 = p1.second;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    int x = x0;
    int y = y0;

    while (true) {
        if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
            int index = (y * BUFFER_WIDTH + x) * BYTES_PER_PIXEL;

            static_rgb_buffer[index + 0] = 255;
            static_rgb_buffer[index + 1] = 255;
            static_rgb_buffer[index + 2] = 255;
        }

        if (x == x1 && y == y1) break;

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }

        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
}


#endif
