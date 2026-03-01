#ifndef _SIMULATE_HPP
#define _SIMULATE_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <climits>

#include "struct.hpp"
#include "settings.hpp"
#include "floaters.hpp"
#include "math.hpp"


// diagnostic stuff!!!
struct SimDiag {
    // Fluid particles
    int   fluid_total   = 0;
    int   fluid_in_box  = 0;
    int   fluid_escaped = 0;
    float fluid_max_v   = 0.f;
    float fluid_max_d   = 0.f;
    float fluid_max_p   = 0.f;
    float fluid_avg_d   = 0.f;
    float fluid_avg_p   = 0.f;

    int   ghost_total        = 0;
    int   ghost_in_grid      = 0;
    int   ghost_max_pressure = 0;
    float ghost_avg_pressure = 0.f;

    int near_left   = 0;
    int near_right  = 0;
    int near_top    = 0;
    int near_bottom = 0;
};

inline SimDiag collectDiagnostics(floater* p, int frame)
{
    SimDiag d;
    const float lo_x = (float)BUFFER_PADDING;
    const float hi_x = (float)(BUFFER_PADDING + BUFFER_WORKING);
    const float lo_y = (float)BUFFER_PADDING;
    const float hi_y = (float)(BUFFER_PADDING + BUFFER_WORKING);
    const float wall_thresh = PARTICLE_SIZE;

    float sum_d_fluid = 0.f, sum_p_fluid = 0.f;
    float sum_p_ghost = 0.f;

    for (size_t i = 0; i < FLOATER_AMT; i++) {
        if (p[i].enabled) {
            d.fluid_total++;
            float vabs = std::abs(p[i].v_x) + std::abs(p[i].v_y);
            d.fluid_max_v = std::max(d.fluid_max_v, vabs);
            d.fluid_max_d = std::max(d.fluid_max_d, p[i].density);
            d.fluid_max_p = std::max(d.fluid_max_p, p[i].pressure);
            sum_d_fluid  += p[i].density;
            sum_p_fluid  += p[i].pressure;

            bool in_box = (p[i].x >= lo_x && p[i].x < hi_x &&
                           p[i].y >= lo_y && p[i].y < hi_y);
            if (in_box)  d.fluid_in_box++;
            else         d.fluid_escaped++;

            if (p[i].x - lo_x < wall_thresh) d.near_left++;
            if (hi_x - p[i].x < wall_thresh) d.near_right++;
            if (p[i].y - lo_y < wall_thresh) d.near_top++;
            if (hi_y - p[i].y < wall_thresh) d.near_bottom++;
        } else {
            d.ghost_total++;
            bool in_grid = (p[i].x >= 0 && p[i].x < (float)BUFFER_WIDTH &&
                            p[i].y >= 0 && p[i].y < (float)BUFFER_HEIGHT);
            if (in_grid) d.ghost_in_grid++;
            if (p[i].pressure > (float)d.ghost_max_pressure)
                d.ghost_max_pressure = (int)p[i].pressure;
            sum_p_ghost += p[i].pressure;
        }
    }

    if (d.fluid_total  > 0) { d.fluid_avg_d = sum_d_fluid / d.fluid_total;
                               d.fluid_avg_p = sum_p_fluid / d.fluid_total; }
    if (d.ghost_total  > 0)   d.ghost_avg_pressure = sum_p_ghost / d.ghost_total;
    return d;
}

inline void printDiagnostics(const SimDiag& d, int frame)
{
    // if there is an error regarding this, rewrite it, there this was written using ai lol
    //
    std::cerr << std::fixed << std::setprecision(3);
    std::cerr << "=== FRAME " << frame << " ===\n";
    std::cerr << "  FLUID  total=" << d.fluid_total
              << "  in-box=" << d.fluid_in_box
              << "  escaped=" << d.fluid_escaped << "\n";
    std::cerr << "         max_v=" << d.fluid_max_v
              << "  max_d=" << d.fluid_max_d
              << "  max_p=" << d.fluid_max_p
              << "  avg_d=" << d.fluid_avg_d
              << "  avg_p=" << d.fluid_avg_p << "\n";
    std::cerr << "  WALLS  near_left=" << d.near_left
              << "  near_right=" << d.near_right
              << "  near_top=" << d.near_top
              << "  near_bottom=" << d.near_bottom << "\n";
    std::cerr << "  GHOST  total=" << d.ghost_total
              << "  in-grid=" << d.ghost_in_grid
              << "  max_p=" << d.ghost_max_pressure
              << "  avg_p=" << d.ghost_avg_pressure << "\n";
    // Warn if ghosts are not registering in the grid at all
    if (d.ghost_total > 0 && d.ghost_in_grid == 0) {
        std::cerr << "  [WARN] NO ghost particles are in the spatial grid! "
                     "Boundary will not work.\n";
    }
    // Warn if large number of fluid escaped
    if (d.fluid_escaped > d.fluid_total / 10) {
        std::cerr << "  [WARN] " << d.fluid_escaped
                  << " fluid particles are outside the working region!\n";
    }
}


namespace JD::simulate
{
    template <auto KernelFunc>
    void computeDensity(int* offsets_in,
                        int* cells_ctr_in,
                        int* particles_loc_in,
                        int  region_amt,          // should be BLOCK_NEIGHBOR_COUNT
                        JD::floaters::block* blocks,  
                        floater* p_floatersA,
                        float h_in)
    {
#pragma omp parallel for num_threads(16)
        for (size_t i = 0; i < FLOATER_AMT; i++)
        {
            int bx = (int)(p_floatersA[i].x / DISTANCE_BETWEEN_POINTS);
            int by = (int)(p_floatersA[i].y / DISTANCE_BETWEEN_POINTS);
            
            float temp = 0.0f;
            float x    = p_floatersA[i].x;
            float y    = p_floatersA[i].y;

            if (bx >= 0 && bx < BUFFER_LINE && by >= 0 && by < BUFFER_LINE) {
                size_t idx = (size_t)(bx + by * BUFFER_LINE);

                for (int j = 0; j < region_amt; j++)
                {
                    int idx_r = (int)blocks[idx].regions[j];
                    if (idx_r == INT_MAX) { continue; }

                    int idx_o = offsets_in[idx_r];

                    for (int k = 0; k < cells_ctr_in[idx_r]; k++)
                    {
                        int floater_idx = particles_loc_in[idx_o + k];
                        float dx = p_floatersA[floater_idx].x - x;
                        float dy = p_floatersA[floater_idx].y - y;
                        float dist_sq = dx*dx + dy*dy;
                    
                        temp += p_floatersA[floater_idx].mass * KernelFunc(dist_sq, h_in);
                    }
                }

                // ghost are on wall, their densities are a bit higher to 
                // ya
                //
                if (!p_floatersA[i].enabled) temp *= 2.0f;

                p_floatersA[i].density  = temp;
                p_floatersA[i].pressure = std::max(0.0f,
                    PARTICLE_BULK_MODULUS * (p_floatersA[i].density - PARTICLE_REFERENCE_DENSITY));
            }
        }
    }


    template <auto KernelGrad>
    void computePressureForce(int* offsets_in,
                              int* cells_ctr_in,
                              int* particles_loc_in,
                              floater* particles_in,
                              float h_in)
    {
#pragma omp parallel for num_threads(16)
        for (size_t i = 0; i < FLOATER_AMT; i++) {
            if (!particles_in[i].enabled) continue; // Ghosts don't receive forces

            float x  = particles_in[i].x;
            float y  = particles_in[i].y;
            int   bx = (int)(x / DISTANCE_BETWEEN_POINTS);
            int   by = (int)(y / DISTANCE_BETWEEN_POINTS);
            
            if (bx >= 0 && bx < BUFFER_LINE && by >= 0 && by < BUFFER_LINE) {
                size_t idx = (size_t)(bx + by * BUFFER_LINE);

                for (int r = 0; r < JD::floaters::BLOCK_NEIGHBOR_COUNT; r++) {
                    int idx_r = (int)JD::floaters::blocks[idx].regions[r];
                    if (idx_r == INT_MAX) continue;

                    int idx_o = offsets_in[idx_r];
                    for (int k = 0; k < cells_ctr_in[idx_r]; k++) {
                        int j = particles_loc_in[idx_o + k];
                        if (i == (size_t)j) continue;

                        float dx    = x - particles_in[j].x;
                        float dy    = y - particles_in[j].y;
                        float r_sq  = dx*dx + dy*dy;

                        float dist = std::sqrt(r_sq);
                        float r_norm = dist/h_in;

                        if (r_sq > 0 && r_sq < h_in*h_in) {
                            if (!particles_in[i].enabled)
                            {
                                if(r_norm < PARTICLE_SIZE)
                                {
                                    float k_repulsion = 2.5;
                                    float force_mag = k_repulsion * (1.0f - r_norm) / (r_sq + 0.01f);

                                    particles_in[i].a_x += force_mag * (dx / dist);
                                    particles_in[i].a_y += force_mag * (dy / dist);
                                
                                    float friction_coeff = 0.1f;
                                    float dvx = particles_in[i].v_x - particles_in[j].v_x; 
                                    float dvy = particles_in[i].v_y - particles_in[j].v_y;

                                    particles_in[i].a_x -= friction_coeff * dvx;
                                    particles_in[i].a_y -= friction_coeff * dvy;
                                }
                            }
                            else {
                                force grad_f;
                                KernelGrad(dx, dy, r_sq, h_in, grad_f);

                                float rho_i = std::max(particles_in[i].density, 1e-6f);
                                float rho_j = std::max(particles_in[j].density, 1e-6f);
                                
                                // monagham thingy ma bober to fix the pressure going BOOM
                                // F_ij = -m_j * (P_i + P_j) / (rho_i * rho_j) * grad_W
                                // Using (P_i+P_j)/(rho_i*rho_j) instead of P/rho^2 keeps

                                float p_i = particles_in[i].pressure;
                                float p_j;
                                p_j = (!particles_in[j].enabled) ? p_i * 1.0f : particles_in[j].pressure;

                                float p_term = (p_i + p_j) / (rho_i * rho_j);

                                particles_in[i].a_x -= particles_in[j].mass * p_term * grad_f.x;
                                particles_in[i].a_y -= particles_in[j].mass * p_term * grad_f.y;
                            }
                        }
                    }
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
#pragma omp parallel for num_threads(16)
        for (size_t i = 0; i < FLOATER_AMT; i++) {
            if (!particles_in[i].enabled) continue;

            float x  = particles_in[i].x;
            float y  = particles_in[i].y;
            int   bx = (int)(x / DISTANCE_BETWEEN_POINTS);
            int   by = (int)(y / DISTANCE_BETWEEN_POINTS);
            
            if (bx >= 0 && bx < BUFFER_LINE && by >= 0 && by < BUFFER_LINE) {
                size_t idx = (size_t)(bx + by * BUFFER_LINE);

                for (int r = 0; r < JD::floaters::BLOCK_NEIGHBOR_COUNT; r++) {
                    int idx_r = (int)JD::floaters::blocks[idx].regions[r];
                    if (idx_r == INT_MAX) continue;

                    int idx_o = offsets_in[idx_r];
                    for (int k = 0; k < cells_ctr_in[idx_r]; k++) {
                        int j = particles_loc_in[idx_o + k];
                        if (i == (size_t)j) continue;

                        float dx   = x - particles_in[j].x;
                        float dy   = y - particles_in[j].y;
                        float r_sq = dx*dx + dy*dy;
                        if (r_sq > 0 && r_sq < h_in*h_in) {
                            float lap   = KernelLap(r_sq, h_in);
                            float rho_j = std::max(particles_in[j].density, 1e-6f);
                            float v_mod = PARTICLE_VISCOSITY * (particles_in[j].mass / rho_j);
                            particles_in[i].a_x += v_mod * (particles_in[j].v_x - particles_in[i].v_x) * lap;
                            particles_in[i].a_y += v_mod * (particles_in[j].v_y - particles_in[i].v_y) * lap;
                        }
                    }
                }
            }
        }
    }

    // apply accel to activ
    template <auto function>
    void applyYAccelerationToAllParticles(floater* particles_in)
    {
        float valueToApply = function();
        for (size_t i = 0; i < FLOATER_AMT; i++)
        {
            if (!particles_in[i].enabled) break;
            particles_in[i].a_y += valueToApply;
        }
    }
    

    void integrate(int* offsets_in,
                   int* cells_ctr_in,
                   int* particles_loc_in,
                   floater* particles_in) 
    {
        const float lo_x = (float)BUFFER_PADDING;
        const float hi_x = (float)(BUFFER_PADDING + BUFFER_WORKING);
        const float lo_y = (float)BUFFER_PADDING;
        const float hi_y = (float)(BUFFER_PADDING + BUFFER_WORKING);

#pragma omp parallel for num_threads(16)
        for (size_t i = 0; i < FLOATER_AMT; i++) 
        {
            if (!particles_in[i].enabled) continue; // ghosts stay fixed

            particles_in[i].v_x += particles_in[i].a_x * PARTICLE_TIME_STEP; 
            particles_in[i].v_y += particles_in[i].a_y * PARTICLE_TIME_STEP;

            // Velocity clamp – prevents tunnelling through boundaries
            if (particles_in[i].v_x >  PARTICLE_MAX_V) particles_in[i].v_x =  PARTICLE_MAX_V;
            if (particles_in[i].v_x < -PARTICLE_MAX_V) particles_in[i].v_x = -PARTICLE_MAX_V;
            if (particles_in[i].v_y >  PARTICLE_MAX_V) particles_in[i].v_y =  PARTICLE_MAX_V;
            if (particles_in[i].v_y < -PARTICLE_MAX_V) particles_in[i].v_y = -PARTICLE_MAX_V;

            particles_in[i].x += particles_in[i].v_x * PARTICLE_TIME_STEP; 
            particles_in[i].y += particles_in[i].v_y * PARTICLE_TIME_STEP;
            particles_in[i].a_x = 0; 
            particles_in[i].a_y = 0;

            
            // one day
            // I will
            // figure out
            // how to 
            // make
            // ghost
            // particles
            // work
            // :D
            // sadness
            // this is a band aid right now sadly
            if (particles_in[i].x < lo_x) {
                particles_in[i].x   = lo_x + (lo_x - particles_in[i].x);
                particles_in[i].v_x = std::abs(particles_in[i].v_x) * PARTICLE_RESTITUTION;
            }
            if (particles_in[i].x >= hi_x) {
                particles_in[i].x   = hi_x - (particles_in[i].x - hi_x) - 1.0f;
                particles_in[i].v_x = -std::abs(particles_in[i].v_x) * PARTICLE_RESTITUTION;
            }
            if (particles_in[i].y < lo_y) {
                particles_in[i].y   = lo_y + (lo_y - particles_in[i].y);
                particles_in[i].v_y = std::abs(particles_in[i].v_y) * PARTICLE_RESTITUTION;
            }
            if (particles_in[i].y >= hi_y) {
                particles_in[i].y   = hi_y - (particles_in[i].y - hi_y) - 1.0f;
                particles_in[i].v_y = -std::abs(particles_in[i].v_y) * PARTICLE_RESTITUTION;
            }

            // Final safety clamp: if still out of bounds after reflection,
            // place back at boundary (extreme edge case only)
            particles_in[i].x = std::max(lo_x, std::min(hi_x - 1.0f, particles_in[i].x));
            particles_in[i].y = std::max(lo_y, std::min(hi_y - 1.0f, particles_in[i].y));
            
        }
    }
}

#endif
