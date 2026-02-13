#include <numbers>

#include "struct.hpp"
#include "poly6.hpp"

float JD::Poly6_k::smoothing(float distance_i, float particle_size_i)
{
    if (distance_i < 0 || distance_i >= particle_size_i) return 0.0f;

    float h = particle_size_i;
    float h2 = h * h;
    float r2 = distance_i;
    float diff = h2 - r2;

    float coeff = 315.0f / (64.0f * std::numbers::pi_v<float> * std::pow(h, 9));
    return coeff * (diff * diff * diff);
}

void JD::Poly6_k::gradient(float dx, float dy, float distance_i, float particle_size_i, force& out_force)
{
    if (distance_i <= 0 || distance_i >= particle_size_i)
    {
        out_force.x = 0.0f; out_force.y = 0.0f;
        return;
    }

    float h = particle_size_i;
    float diff = (h * h) - (distance_i);
    float coeff = 4.0f / (std::numbers::pi_v<float> * std::pow(h, 8));

    float scalar = coeff * -6.0f * (diff * diff);

    // return~ish
    out_force.x = scalar * dx;
    out_force.y = scalar * dy;
}

float JD::Poly6_k::laplacian(float distance_i, float particle_size_i) {
    if (distance_i < 0 || distance_i >= particle_size_i) return 0.0f;

    float h = particle_size_i;
    float h2 = h * h;
    float r2 = distance_i;
    float coeff = 4.0f / (std::numbers::pi_v<float> * std::pow(h, 8));

    return coeff * -6.0f * (3.0f * h2 * h2 - 10.0f * h2 * r2 + 7.0f * r2 * r2);
}
