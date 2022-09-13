#pragma once

#include <cmath>
#include <limits>
#include <cstdlib>

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

// Utility Functions

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0f;
}


// Returns a random real in [0,1).
inline float random_float() {
	return rand() / (RAND_MAX + 1.0f);
}

// Returns a random real in [min,max).
inline float random_float(float min, float max) {
	return min + (max - min) * random_float();
}

inline float clamp(float x, float min, float max) {
	return fmax(fmin(x, max), min);
}
