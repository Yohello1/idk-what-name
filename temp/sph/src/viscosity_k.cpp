#include <numbers>

#include "struct.hpp"
#include "viscosity_k.hpp"

float JD:: Viscosity_k::laplacian(float distance_i, float particle_size_i)
{
    if (distance_i < 0 || distance_i >= particle_size_i) return 0.0f;

    float h = particle_size_i;
    float coeff = 40.0f / (std::numbers::pi_v<float> * std::pow(h, 5));

    return coeff * (h - distance_i);
}
