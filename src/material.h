#pragma once

#include "ray.h"
#include "vec3.h"

struct hit_info_t;

class material_t {
public:
	virtual bool scatter(const ray_t& ray_in, hit_info_t& hit_info, color_t& attentuation, ray_t& scatter_ray) const = 0;
};
