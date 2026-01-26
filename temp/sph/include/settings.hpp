#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include <stdint.h>
#include <cmath>

inline constexpr int DISTANCE_BETWEEN_POINTS = 8;

inline constexpr int SIZE_MULTIPLIER = 90;
inline constexpr int INFLUENCE_RADIUS = 9; // This is how far we look in terms of blocks lol
inline constexpr int PADDING = std::max(3, INFLUENCE_RADIUS+1);

inline constexpr int BUFFER_WIDTH = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS+(DISTANCE_BETWEEN_POINTS*PADDING*2);
inline constexpr int BUFFER_HEIGHT = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS+(DISTANCE_BETWEEN_POINTS*PADDING*2);

inline constexpr int BUFFER_PADDING = PADDING*DISTANCE_BETWEEN_POINTS;
inline constexpr int BUFFER_UNPADDED = PADDING*DISTANCE_BETWEEN_POINTS + SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;
inline constexpr int BUFFER_LINE = PADDING*2 + SIZE_MULTIPLIER;
inline constexpr int BUFFER_WORKING = SIZE_MULTIPLIER*DISTANCE_BETWEEN_POINTS;

inline constexpr int BYTES_PER_PIXEL = 3; // R, G, B
inline constexpr int SCREEN_SCALE = 1;

inline constexpr int POINTS_WIDTH = SIZE_MULTIPLIER+1;
inline constexpr int POINTS_HEIGHT = SIZE_MULTIPLIER+1;
inline constexpr int POINTS_AMT = (POINTS_HEIGHT)*(POINTS_WIDTH);

inline constexpr int FLOATER_AMT = 1000;
inline constexpr int FLOATER_SPEED = 3;

inline constexpr uint16_t THRESHOLD = 180;

// Fluid simulation global constexprants
inline const float PARTICLE_SIZE = 1.0f; // otherwise known as the influence radisu
inline const float PARTICLE_TIME_STEP = 1.0f;
inline const float PARTICLE_REFERENCE_DENSITY = 1.0f;
inline const float PARTICLE_BULK_MODULUS = 1.0f;
inline const float PARTICLE_VISCOSITY = 1.0f;
inline const float PARTICLE_GRAVITY = 1.0f;
inline const float PARTICLE_MASS = 1.0f;

inline const float GRAVITY_CONSTANT = 10.0f;

inline const int PARTICLE_N_FRAMES = 0; // Number of frames
inline const int PARTICLE_NP_FRAMES = 0; // Steps per frame


constexpr int CELL_SIZE = DISTANCE_BETWEEN_POINTS*DISTANCE_BETWEEN_POINTS;

inline const int REGIONS_AMT = 2*(INFLUENCE_RADIUS)*(INFLUENCE_RADIUS)-2*INFLUENCE_RADIUS + 1;

#endif
