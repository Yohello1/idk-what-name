#ifndef _SIMULATE_HPP
#define _SIMULATE_HPP

#include <vector>
#include <cmath>
#include <iostream>

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
                        int region_amt,
                        JD::floaters::block* blocks,  
                        floater* p_floatersA,
                        float h_in) // Particle size 
    {
        // I will set the correct lookup method after....
        /*
        for (int i = 0; i < FLOATER_AMT; i++) 
        {
            float temp = 0.0f;

            float x = p_floatersA[i].x;
            float y = p_floatersA[i].y;

            for(int j = 0; j < region_amt; j++) 
            {
                int idx_r = JD::graphics::points[i].regions[j];
                int idx_o = JD::graphics::offsets[idx_r];

                for(int k = 0; k < cells_ctr_in[idx_r]; k++) 
                {
                    int floater_idx = particles_loc_in[idx_o + k];
                    float dx = p_floatersA[floater_idx].x - x;
                    float dy = p_floatersA[floater_idx].y - y;
                    float dist_sq = dx*dx + dy*dy;
                
                    temp += p_floatersA[j].mass * KernelFunc(dist_sq, h_in);
                }
            }

            p_floatersA[i].density = temp;
            // put here for future use, makes compute a bit cheaper
            p_floatersA[i].pressure = PARTICLE_BULK_MODULUS * (p_floatersA[i].density - PARTICLE_REFERENCE_DENSITY);
        }
        */
        // itterate through all points that are ON
        // determine block it belongs to
        // access those blocks from there
        // and itterate based on that
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            int bx = p_floatersA[i].x/DISTANCE_BETWEEN_POINTS;
            int by = p_floatersA[i].y/DISTANCE_BETWEEN_POINTS;
            size_t idx = bx + by*BUFFER_LINE;
            float x = p_floatersA[i].x;
            float y = p_floatersA[i].y;


            float temp = 0.0f;
            for(int j = 0; j < region_amt; j++)
            {
                // now we look at blocks[idx].regions[j]
                int idx_r = blocks[idx].regions[j];
                if(idx_r == INT_MAX) { continue;} 

                int idx_o = JD::graphics::offsets[idx_r];

                for(int k = 0; k < cells_ctr_in[idx_r]; k++)
                {
                    int floater_idx = particles_loc_in[idx_o + k];
                    float dx = p_floatersA[floater_idx].x - x;
                    float dy = p_floatersA[floater_idx].y - y;
                    float dist_sq = dx*dx + dy*dy;
                
                    temp += p_floatersA[j].mass * KernelFunc(dist_sq, h_in);
                }

            }
            p_floatersA[i].density = temp;
            // put here for future use, makes compute a bit cheaper
            p_floatersA[i].pressure = PARTICLE_BULK_MODULUS * (p_floatersA[i].density - PARTICLE_REFERENCE_DENSITY);
        }
    }


    template <auto KernelGrad>
    void computePressureForce(int* offsets_in,
                              int* cells_ctr_in,
                              int* particles_loc_in,
                              floater* particles_in,
                              float h_in) // Particle size 
    {
        for (size_t i = 0; i < FLOATER_AMT; i++) {
            for (size_t j = 0; j < FLOATER_AMT; j++) {
                if (i == j) continue;
                float dx = particles_in[i].x - particles_in[j].x;
                float dy = particles_in[i].y - particles_in[j].y;
                float r = (dx*dx + dy*dy);

                if (r > 0 && r < h_in*h_in) {
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
        for (size_t i = 0; i < FLOATER_AMT; i++) {
            for (size_t j = 0; j < FLOATER_AMT; j++) {
                if (i == j) continue;
                float dx = particles_in[i].x - particles_in[j].x;
                float dy = particles_in[i].y - particles_in[j].y;
                float r = (dx*dx + dy*dy);
                if (r > 0 && r < h_in*h_in) {
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
        for(size_t i = 0; i < FLOATER_AMT; i++)
        {
            particles_in[i].a_y += valueToApply;
        }
    }
    

    void integrate(int* offsets_in,
                   int* cells_ctr_in,
                   int* particles_loc_in,
                   floater* particles_in) 
    {
        for (size_t i = 0; i < FLOATER_AMT; i++) 
        {
            // skip disabled particles
            if(particles_in[i].enabled == false)
                continue;
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
