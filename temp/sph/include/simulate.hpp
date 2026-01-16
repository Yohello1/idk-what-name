#ifndef _SIMULATE_HPP
#define _SIMULATE_HPP

#include <vector>
#include <cmath>

#include "structs.hpp"
#include "settings.hpp"
#include "floaters.hpp"

template<typename T>
void simulate(const int* offsets_in, 
              const int* cells_ctr_in, 
              const int* particles_loc_in, 
              const int buffer_line_size_in, 
              const int floater_amt_in, 
              const float h_in,
              struct floater particles_in, 
              T& kernel) // can you even make the template const??? 
{
    // density & pressure
    // Im gonna loop through all particles now, and use the proper lookup method later
    for(int i = 0; i < floater_amt_in; i++)
    {
        for(int j = 0; j < floater_amt_in; j++)
        {
            float dx = particles_in[i].x - particles_in[j].x;
            float dy = particles_in[i].x - particles_in[j].x;

            float r = std::sqrt(dx*dx+dy*dy);
            
            if(r < h)
            {
                particles_in[i].density += particles_in[j].mass * kernel.smoothing(r,h);
            }
        }
        particles_in[i].pressure = PARTICLE_BULK_MODULUS * (particles_in[i].density * PARTICLE_REFERENCE_DENSITY); 

    }


}
