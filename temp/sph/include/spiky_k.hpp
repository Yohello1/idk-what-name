#ifndef _SPIKY_K_HPP
#define _SPIKY_K_HPP

#include "struct.hpp" 

namespace JD::Spiky_k {
  void gradient(float dx, float dy, float distance_i, float particle_size_i, force& out_force);
};

#endif
