#ifndef _FLOATERS_HPP
#define _FLOATERS_HPP

#include <climits>

#include "ghost.hpp"
#include "struct.hpp"
#include "settings.hpp"

inline constexpr size_t DESIRED_FLOATERS = 1000;
static inline constexpr size_t GHOST_FLOATERS = 11584;  // should be a function/equation but I cannot rn make it up
                                                    // siracha at 3:45am
inline constexpr size_t FLOATER_AMT = DESIRED_FLOATERS + GHOST_FLOATERS;  
inline constexpr int FLOATER_SPEED = 3;

namespace JD::floaters
{
    static constexpr inline size_t BLOCK_AMT = BUFFER_LINE*BUFFER_LINE; 
   
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
