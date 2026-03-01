#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include <stdint.h>
#include <cmath>

// #include "floaters.hpp"

inline constexpr int DISTANCE_BETWEEN_POINTS = 8;

inline constexpr int SIZE_MULTIPLIER = 20;
inline constexpr int INFLUENCE_RADIUS = 4; // kernel look-ahead in grid cells; increase for more particle sensitivity
inline constexpr int PADDING = std::max(INFLUENCE_RADIUS+1, INFLUENCE_RADIUS+1);

inline constexpr int BUFFER_WIDTH  = SIZE_MULTIPLIER *DISTANCE_BETWEEN_POINTS +(DISTANCE_BETWEEN_POINTS*PADDING*2);
inline constexpr int BUFFER_HEIGHT = SIZE_MULTIPLIER *DISTANCE_BETWEEN_POINTS +(DISTANCE_BETWEEN_POINTS*PADDING*2);

inline constexpr int BUFFER_PADDING = PADDING*DISTANCE_BETWEEN_POINTS;
inline constexpr int BUFFER_UNPADDED = PADDING*DISTANCE_BETWEEN_POINTS + SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;
inline constexpr int BUFFER_LINE = PADDING*2 + SIZE_MULTIPLIER;
inline constexpr int BUFFER_WORKING = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;

inline constexpr int BYTES_PER_PIXEL = 3; // R, G, B
inline constexpr int SCREEN_SCALE = 1;

inline constexpr int POINTS_WIDTH = SIZE_MULTIPLIER+1;
inline constexpr int POINTS_HEIGHT = SIZE_MULTIPLIER+1;
inline constexpr int POINTS_AMT = (POINTS_HEIGHT)*(POINTS_WIDTH);

inline constexpr float THRESHOLD = 0.0f;

// Window settings
const int WINDOW_WIDTH = 784;
const int WINDOW_HEIGHT = 784;

//   actual resting density ≈ 0.18 (Poly6 self-contribution * neighbour count)
//   keep reference density close to expected resting density to minimise rest pressure
inline const float PARTICLE_SIZE = INFLUENCE_RADIUS * DISTANCE_BETWEEN_POINTS; // kernel radius px
inline const float PARTICLE_TIME_STEP = 0.5f;           // smaller step = more stable
inline const float PARTICLE_REFERENCE_DENSITY = 0.015f;  // target resting density
inline const float PARTICLE_BULK_MODULUS = 40.0f;       // lower = softer / less explosive
inline const float PARTICLE_VISCOSITY = 0.5f;           // reduced damping for more fluid motion
inline const float PARTICLE_GRAVITY = 0.5f;
inline const float PARTICLE_MASS = 1.0f;                // normalised mass; pressure formula handles scaling

// until i learn how to write code :(
inline const float PARTICLE_MAX_V = 7.5f;
inline const float PARTICLE_RESTITUTION = 0.5f;

inline const int PARTICLE_GHOST_DENSITY = 8.0;

inline const float GRAVITY_CONSTANT = 0.1f;

inline const int PARTICLE_N_FRAMES = 0; // Number of frames
inline const int PARTICLE_NP_FRAMES = 0; // Steps per frame


constexpr int CELL_SIZE = DISTANCE_BETWEEN_POINTS*DISTANCE_BETWEEN_POINTS;

inline const int REGIONS_AMT = 2*(INFLUENCE_RADIUS)*(INFLUENCE_RADIUS)-2*INFLUENCE_RADIUS + 1;

#endif
