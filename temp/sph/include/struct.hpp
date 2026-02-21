
#ifndef _STRUCT_HPP

#define _STRUCT_HPP
#include <stdint.h>
#include "settings.hpp"

struct point
{
    uint16_t x;
    uint16_t y;

    uint16_t i_x;
    uint16_t i_y;

    uint16_t id;
    uint16_t strength;

    int regions[REGIONS_AMT];

};

struct floater {
    float density;

    float p_x;
    float p_y;

    float x;
    float y;

    float v_x; // full
    float v_y; // full

    float v_x_h; // half
    float v_y_h; // half

    float a_x;
    float a_y;

    float mass;
    float pressure;

    bool enabled; // true: enabled, false: disabled
};

struct force {
    float x;
    float y;

};
#endif
