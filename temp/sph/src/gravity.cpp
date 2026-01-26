#include <cmath>

#include "settings.hpp"
#include "gravity.hpp"

namespace JD::gravity 
{
    float gravityAcceleration()
    {
        return PARTICLE_MASS * GRAVITY_CONSTANT; 
    }
}
