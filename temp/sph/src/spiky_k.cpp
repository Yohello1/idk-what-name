#include <numbers>

#include "struct.hpp"
#include "spiky_k.hpp"

void JD::Spiky_k::gradient(float dx, float dy, float distance_i, float particle_size_i, force& out_force)
{
    if (distance_i <= 0 || distance_i >= particle_size_i) {
        out_force.x = 0.0f; out_force.y = 0.0f;
        return;
    }

    float h = particle_size_i;
    float diff = h - distance_i;

    float coeff = -10.0f / (std::numbers::pi_v<float> * std::pow(h, 5));

    float scalar = (coeff * diff * diff) / distance_i;
    out_force.x = scalar * dx;
    out_force.y = scalar * dy;
}
