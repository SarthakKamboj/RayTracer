#pragma once

#include "vec3.h"
#include "ray.h"
#include <memory>

class material_t;

struct hit_info_t {
	point_t point;
	vec3_t normal;
	float t;
	std::shared_ptr<material_t> mat_ptr;
};

class hittable {
public:
	virtual bool hit(const ray_t& ray, float t_min, float t_max, hit_info_t& hit_info) const = 0;
};
