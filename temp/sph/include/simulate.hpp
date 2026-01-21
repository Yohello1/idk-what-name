#ifndef _SIMULATE_HPP
#define _SIMULATE_HPP

#include <vector>
#include <cmath>

#include "structs.hpp"
#include "settings.hpp"
#include "floaters.hpp"


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
        for (int i = 0 i < FLOATER_AMT; i++) {
            float temp = 0.0f;
            for (int j = 0; j < FLOATER_AMT; j++) {
                float r = fdistEuclid(particles_in[i], particles_in[j]);
                if (r < h_in) temp += particles_in[j].mass * KernelFunc(r, h);
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
                float dx = particles_in[i].x - pj.x;
                float dy = particles_in[i].y - pj.y;
                float r = std::sqrt(dx*dx + dy*dy);

                if (r > 0 && r < h) {
                    force grad_w;
                    KernelGrad(dx, dy, r, h, grad_f);
                    float p_term = (pi.pressure + pj.pressure) / (2.0f * pj.density);
                    pi.ax -= pj.mass * p_term * grad_f.x;
                    pi.ay -= pj.mass * p_term * grad_f.y;
                }
            }
        }
    }

    template <auto KernelLap>
    void compute_viscosity(std::vector<particle>& particles, float h) {
        for (auto& pi : particles) {
            for (auto& pj : particles) {
                if (&pi == &pj) continue;
                float r = math_dist(pi, pj);
                if (r > 0 && r < h) {
                    float lap = KernelLap(r, h);
                    float v_mod = SETTINGS.visc_mu * (pj.mass / pj.density);
                    pi.ax += v_mod * (pj.vx - pi.vx) * lap;
                    pi.ay += v_mod * (pj.vy - pi.vy) * lap;
                }
            }
        }
    }


    void integrate(std::vector<particle>& particles, float dt) {
        for (auto& p : particles) {
            p.vx += p.ax * dt; p.vy += p.ay * dt;
            p.x += p.vx * dt; p.y += p.vy * dt;
            p.ax = 0; p.ay = 0;
        }
    }
}

