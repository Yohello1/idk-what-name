#ifndef _FLOATERS_HPP
#define _FLOATERS_HPP

#include <climits>

#include "ghost.hpp"
#include "struct.hpp"
#include "settings.hpp"

inline constexpr size_t DESIRED_FLOATERS = 1000;
static inline constexpr size_t ghostCalcAmt() {size_t i = PADDING*DISTANCE_BETWEEN_POINTS*BUFFER_WORKING*4 + PADDING*PADDING*4; return i;};
inline constexpr size_t FLOATER_AMT = DESIRED_FLOATERS + ghostCalcAmt(); 
inline constexpr int FLOATER_SPEED = 3;

namespace JD::floaters
{
    static constexpr inline size_t BLOCK_AMT = (SIZE_MULTIPLIER+PADDING)*(SIZE_MULTIPLIER+PADDING);     
   
    struct block
    {
        uint32_t regions[9];
    };

    extern floater* floatersA;

    extern block*   blocks; 

    void init();

    void initFloaters();
    void drawFloaters();

    void initBlockRegions();

}


#endif // _FLOATERS_HPP
