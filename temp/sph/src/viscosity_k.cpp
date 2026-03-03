#include <numbers>

#include "struct.hpp"
#include "viscosity_k.hpp"
#include "math.hpp"

float JD:: Viscosity_k::laplacian(float distance_i, float particle_size_i)
{
    float h = particle_size_i;
    float h2 = h * h;
    if (distance_i < 0 || distance_i >= h2) return 0.0f;

    float coeff = (1/( std::pow(h, 6))) * PARTICLE_VISCOSITY_K_COEFF;

    return coeff * (h - std::sqrt(distance_i));
}
