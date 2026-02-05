#ifndef _GHOST_HPP
#define _GHOST_HPP

#include <stdint.h>
#include <cmath>

#include "settings.hpp" 

namespace JD::ghost
{
    // Need very basic method to just fill in stuff otuside of the boundary with ghost particles
    constexpr size_t ghostCalcAmt() {size_t i = PADDING*DISTANCE_BETWEEN_POINTS*BUFFER_WORKING*4 + PADDING*PADDING*4; return i;};

}

#endif // _GHOST_HPP
