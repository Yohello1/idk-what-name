#ifndef _MATH_HPP
#define _MATH_HPP

#include <vector>
#include <utility>
#include "struct.hpp"

namespace JD::math {

    int signBit(int a);
    float fsignBit(float a);
    float fdistEuclid(std::vector<float> a, std::vector<float> b);
    std::pair<int, int> getMidPoint(const point& p0, const point& p1);
    float rsqrt(float number);
} // namespace JD::math

#endif
