#ifndef _MATH_CPP
#define _MATH_CPP

#include <vector>
#include <utility>
#include "struct.hpp"

int signBit(int a);
float fsignBit(float a);
float fdistEuclid(std::vector<float> a, std::vector<float> b);
std::pair<int, int> get_mid_point(const point& p0, const point& p1);

#endif
