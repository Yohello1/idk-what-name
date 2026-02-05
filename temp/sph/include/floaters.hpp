#ifndef _FLOATERS_HPP
#define _FLOATERS_HPP

#include "ghost.hpp"
#include "struct.hpp"

inline constexpr size_t DESIRED_FLOATERS = 1000;
inline constexpr size_t FLOATER_AMT = DESIRED_FLOATERS + JD::ghost::ghostCalcAmt(); 
inline constexpr int FLOATER_SPEED = 3;

namespace JD::floaters
{

    floater* floatersA = new floater[ FLOATER_AMT ];
    floater* floatersB = new floater[ FLOATER_AMT ];
    
    void initFloaters();
    void drawFloaters();
}


#endif // _FLOATERS_HPP
