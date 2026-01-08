#ifndef _MATH_CPP
#define _MATH_CPP

#include "math.hpp"
#include "struct.hpp"
#include <math.h>
#include <vector>


int signBit(int a) { return a/abs(a); };
float fsignBit(float a) { return a/abs(a); };

float fdistEuclid(std::vector<float> a, std::vector<float> b)
{
    float qq = 0.0f;

    for(size_t i = 0; i < a.size(); i++)
    {
        qq += (a[i]-b[i])*(a[i]-b[i]);
    }

    return std::sqrt(qq);
}

std::pair<int, int> getMidPoint(const point& p0, const point& p1)
{
    return {(p0.i_x + p1.i_x) / 2, (p0.i_y + p1.i_y) / 2};
}



#endif
