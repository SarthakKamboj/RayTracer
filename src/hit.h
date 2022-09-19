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
	bool front_face;

	void set_front_face(const ray_t& ray, const vec3_t& outward_normal) {
		front_face = dot(ray.dir, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray_t& ray, float t_min, float t_max, hit_info_t& hit_info) const = 0;
};
