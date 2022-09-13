#pragma once

#include "vec3.h"

class ray_t {
public:
	ray_t() {}
	ray_t(point_t& _origin, vec3_t& _dir) : origin(_origin), dir(_dir) {}

	point_t origin;
	vec3_t dir;

	point_t at(float x) const {
		return origin + (x * dir);
	}
};
