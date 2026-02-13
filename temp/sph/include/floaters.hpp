#ifndef _FLOATERS_HPP
#define _FLOATERS_HPP

#include "ghost.hpp"
#include "struct.hpp"

inline constexpr size_t DESIRED_FLOATERS = 1000;
static inline constexpr size_t ghostCalcAmt() {size_t i = PADDING*DISTANCE_BETWEEN_POINTS*BUFFER_WORKING*4 + PADDING*PADDING*4; return i;};
inline constexpr size_t FLOATER_AMT = DESIRED_FLOATERS + ghostCalcAmt(); 
inline constexpr int FLOATER_SPEED = 3;

namespace JD::floaters
{
    extern floater* floatersA;
    // extern floater* floatersB;
    
    void initFloaters();
    void drawFloaters();
}


#endif // _FLOATERS_HPP
