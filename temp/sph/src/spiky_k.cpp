#include <numbers>

#include "struct.hpp"
#include "spiky_k.hpp"
#include "math.hpp"

void JD::Spiky_k::gradient(float dx, float dy, float distance_i, float particle_size_i, force& out_force)
{
    if (distance_i <= 0 || distance_i >= particle_size_i*particle_size_i) {
        out_force.x = 0.0f; out_force.y = 0.0f;
        return;
    }

    float h = particle_size_i;
    float diff = h - distance_i;

    float coeff = -45.0f / (std::numbers::pi_v<float> * std::pow(h, 6));

    float scalar = (coeff * diff * diff) * JD::math::rsqrt(distance_i) ;
    out_force.x = scalar * dx;
    out_force.y = scalar * dy;
}
