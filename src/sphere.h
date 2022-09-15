#pragma once

#include "vec3.h"
#include "hit.h"
#include <memory>
#include "material.h"

class sphere_t : public hittable {
public:
	sphere_t(point_t _center, float _radius, std::shared_ptr<material_t> _mat) : center(_center), radius(_radius), mat(_mat) {}

	virtual bool hit(const ray_t& ray, float t_min, float t_max, hit_info_t& hit_info) const override {
		float a = dot(ray.dir, ray.dir);
		vec3_t world_orig_to_sphere_center = ray.origin - center;
		float b = 2.0f * dot(ray.dir, world_orig_to_sphere_center);
		float c = (float)dot(world_orig_to_sphere_center, world_orig_to_sphere_center) - (radius * radius);
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0) {
			return false;
		}

		float disc_sqrt = std::sqrt(discriminant);
		float val1 = (-b + disc_sqrt) / (2 * a);
		float val2 = (-b - disc_sqrt) / (2 * a);

		if ((val1 < t_min && val2 < t_min)
			|| (val1 > t_max && val2 > t_max)
			|| (val1 < t_min && val2 > t_max)
			|| (val1 > t_max && val2 < t_min)
			) {
			return false;
		}

		point_t hit1 = ray.at(val1);
		point_t hit2 = ray.at(val2);

		vec3_t normal1 = hit1 - center;
		vec3_t normal2 = hit2 - center;

		float hit_dot1 = dot(normal1, hit1 - ray.origin);
		float hit_dot2 = dot(normal2, hit2 - ray.origin);

		if (hit_dot1 < 0.0f) {
			hit_info.point = hit1;
			hit_info.normal = unit_vector(normal1);
			hit_info.t = val1;
		}
		else {
			hit_info.point = hit2;
			hit_info.normal = unit_vector(normal2);
			hit_info.t = val2;
		}

		hit_info.mat_ptr = mat;

		return true;
	}

	point_t center;
	float radius;
	std::shared_ptr<material_t> mat;
};
