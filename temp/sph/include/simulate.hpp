#ifndef _SIMULATE_HPP
#define _SIMULATE_HPP

#include <vector>
#include <cmath>

#include "struct.hpp"
#include "settings.hpp"
#include "floaters.hpp"
#include "math.hpp"


namespace JD::simulate
{
    template <auto KernelFunc>
    void computeDensity(int* offsets_in,
                        int* cells_ctr_in,
                        int* particles_loc_in,
                        floater* particles_in,
                        float h_in) // Particle size 
    {
        // I will set the correct lookup method after....
        for (int i = 0; i < FLOATER_AMT; i++) {
            float temp = 0.0f;
            for (int j = 0; j < FLOATER_AMT; j++) {
                float r = JD::math::fdistEuclid({particles_in[i].x, particles_in[i].y}, {particles_in[j].x, particles_in[j].y});
                if (r < h_in) temp += particles_in[j].mass * KernelFunc(r, h_in);
            }
            particles_in[i].density = temp;
            // put here for future use, makes compute a bit cheaper
            particles_in[i].pressure = PARTICLE_BULK_MODULUS * (particles_in[i].density - PARTICLE_REFERENCE_DENSITY);
        }
    }


    template <auto KernelGrad>
    void computePressureForce(int* offsets_in,
                              int* cells_ctr_in,
                              int* particles_loc_in,
                              floater* particles_in,
                              float h_in) // Particle size 
    {
        for (int i = 0; i < FLOATER_AMT; i++) {
            for (int j = 0; j < FLOATER_AMT; j++) {
                if (i == j) continue;
                float dx = particles_in[i].x - particles_in[j].x;
                float dy = particles_in[i].y - particles_in[j].y;
                float r = std::sqrt(dx*dx + dy*dy);

                if (r > 0 && r < h_in) {
                    force grad_f;
                    KernelGrad(dx, dy, r, h_in, grad_f);
                    float p_term = (particles_in[i].pressure + particles_in[j].pressure) / (2.0f * particles_in[j].density);
                    particles_in[i].a_x -= particles_in[j].mass * p_term * grad_f.x;
                    particles_in[i].a_y -= particles_in[j].mass * p_term * grad_f.y;
                }
            }
        }
    }

    template <auto KernelLap>
    void computeViscosity(int* offsets_in,
                           int* cells_ctr_in,
                           int* particles_loc_in,
                           floater* particles_in,
                           float h_in) {
        for (int i = 0; i < FLOATER_AMT; i++) {
            for (int j = 0; j < FLOATER_AMT; j++) {
                if (i == j) continue;
                float r = JD::math::fdistEuclid({particles_in[i].x, particles_in[j].y}, {particles_in[j].x, particles_in[j].y});
                if (r > 0 && r < h_in) {
                    float lap = KernelLap(r, h_in);
                    float v_mod = PARTICLE_VISCOSITY * (particles_in[j].mass / particles_in[j].density);
                    particles_in[i].a_x += v_mod * (particles_in[j].v_x - particles_in[i].v_x) * lap;
                    particles_in[i].a_y += v_mod * (particles_in[j].v_y - particles_in[i].v_y) * lap;
                }
            }
        }
    }

    template <auto function>
    void applyYAccelerationToAllParticles(floater* particles_in)
    {
        float valueToApply = function();
        for(int i = 0; i < FLOATER_AMT; i++)
        {
            particles_in[i].a_y += valueToApply;
        }
    }
    

    void integrate(int* offsets_in,
                   int* cells_ctr_in,
                   int* particles_loc_in,
                   floater* particles_in) {
        for (int i = 0; i < FLOATER_AMT; i++) {
            particles_in[i].v_x += particles_in[i].a_x * PARTICLE_TIME_STEP; 
            particles_in[i].v_y += particles_in[i].a_y * PARTICLE_TIME_STEP;
            particles_in[i].x += particles_in[i].v_x * PARTICLE_TIME_STEP; 
            particles_in[i].y += particles_in[i].v_y * PARTICLE_TIME_STEP;
            particles_in[i].a_x = 0; 
            particles_in[i].a_y = 0;
        }
    }
}

#endif
