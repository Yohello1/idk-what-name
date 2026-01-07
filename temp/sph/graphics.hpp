#ifndef _GRAPHICS_HPP
#define _GRAPHICS_HPP

#include "settings.hpp"
#include "struct.hpp"

#include <stdint.h>
#include <utility>



void draw_line_std_pair(uint8_t* buffer,
                        std::pair<int, int> p0,
                        std::pair<int, int> p1,
                        uint8_t r, uint8_t g, uint8_t b);


uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];

int offsets[BUFFER_LINE * BUFFER_LINE];
int cells_ctr[BUFFER_LINE * BUFFER_LINE];
int particles_loc[FLOATER_AMT];

struct point*   points = new struct point[POINTS_AMT];
struct floater* floatersA = new struct floater[FLOATER_AMT];
struct floater* floatersB = new struct floater[FLOATER_AMT];


#endif
