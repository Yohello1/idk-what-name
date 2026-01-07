#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <stdint.h>
#include <utility>
#include "settings.hpp"
#include "struct.hpp"

extern uint8_t static_rgb_buffer[BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL];
extern int offsets[BUFFER_LINE * BUFFER_LINE];
extern int cells_ctr[BUFFER_LINE * BUFFER_LINE];
extern int particles_loc[FLOATER_AMT];

extern point* points;
extern floater* floatersA;
extern floater* floatersB;

void draw_line_std_pair(uint8_t* buffer,
                        std::pair<int, int> p0,
                        std::pair<int, int> p1,
                        uint8_t r, uint8_t g, uint8_t b);
#endif
