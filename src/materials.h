#pragma once

#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "helper.h"

#if 1
class lambertian_t : public material_t {
public:
	lambertian_t(const color_t& _albedo) : albedo(_albedo) {}

	bool scatter(const ray_t& ray_in, hit_info_t& hit_info, color_t& attentuation, ray_t& scatter_ray) const {
		vec3_t scatter_dir = hit_info.point + random_unit_vector();

		if (scatter_dir.near_zero()) scatter_dir = hit_info.normal;

		scatter_ray = ray_t(hit_info.point, scatter_dir);
		attentuation = albedo;
		return true;
	}

private:
	color_t albedo;
};
#endif

class metal_t : public material_t {
public:
	metal_t(const color_t& _albedo) : albedo(_albedo) {}

	virtual bool scatter(const ray_t& ray_in, hit_info_t& hit_info, color_t& attentuation, ray_t& scatter_ray) const override {
#if 0
		vec3_t scatter_dir(1, 1, 1);
		scatter_ray = ray_t(hit_info.point, scatter_dir);
		attentuation = albedo;
		return true;
#else
		vec3_t scatter_dir = reflect(ray_in.dir, hit_info.normal);
		scatter_ray = ray_t(hit_info.point, scatter_dir);
		vec3_t s = vec3_t(0, 1, 0);
		attentuation = albedo;
		return dot(scatter_ray.dir, hit_info.normal) > 0;
#endif
	}

private:
	color_t albedo;
};

