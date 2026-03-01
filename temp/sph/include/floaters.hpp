#ifndef _FLOATERS_HPP
#define _FLOATERS_HPP

#include <climits>

#include "ghost.hpp"
#include "struct.hpp"
#include "settings.hpp"

inline constexpr size_t DESIRED_FLOATERS = 4000;
static inline constexpr size_t GHOST_FLOATERS = 200000;  // should be a function/equation but I cannot rn make it up
                                                    // siracha at 3:45am
inline constexpr size_t FLOATER_AMT = DESIRED_FLOATERS + GHOST_FLOATERS;  
inline constexpr int FLOATER_SPEED = 3;

namespace JD::floaters
{
    static constexpr inline size_t BLOCK_AMT = BUFFER_LINE*BUFFER_LINE;
    static constexpr inline int BLOCK_NEIGHBOR_DIM   = 2 * INFLUENCE_RADIUS + 1;
    static constexpr inline int BLOCK_NEIGHBOR_COUNT = BLOCK_NEIGHBOR_DIM * BLOCK_NEIGHBOR_DIM;
   
    struct block
    {
        uint32_t regions[BLOCK_NEIGHBOR_COUNT];
    };

    extern floater* floatersA;
    extern block*   blocks; 

    void init();

    void initFloaters();
    void drawFloaters();

    void initBlockRegions();

}


#endif // _FLOATERS_HPP
