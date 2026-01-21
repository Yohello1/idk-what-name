#ifndef _POLY6_HPP
#define _POLY6_HPP

#include "struct.hpp"



namespace JD::Poly6_k
{
    float smoothing(float distance_i, float particle_size_i);
    // we modify the supplied struct for the sake of speed
    void gradient(float dx, float dy, float distance_i, float particle_size_i, force& out_force);
    float laplacian(float distance_i, float particle_size_i);
};

#endif
