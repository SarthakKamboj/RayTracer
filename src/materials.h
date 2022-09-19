#pragma once

#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "helper.h"

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

class metal_t : public material_t {
public:
	metal_t(const color_t& _albedo) : albedo(_albedo) {}

	bool scatter(const ray_t& ray_in, hit_info_t& hit_info, color_t& attentuation, ray_t& scatter_ray) const {
		vec3_t scatter_dir = reflect(ray_in.dir, hit_info.normal);
		scatter_ray = ray_t(hit_info.point, scatter_dir);
		attentuation = albedo;
		return dot(scatter_ray.dir, hit_info.normal) > 0;
	}

private:
	color_t albedo;
};

class dielectric_t : public material_t {
public:
	dielectric_t(float index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const ray_t& ray_in, hit_info_t& hit_info, color_t& attenuation, ray_t& scattered) const {
		attenuation = color_t(1.0, 1.0, 1.0);
		float refraction_ratio = hit_info.front_face ? (1.0 / ir) : ir;

		vec3_t unit_direction = unit_vector(ray_in.dir);

		float cosine = fmin(dot(-unit_direction, hit_info.normal), 1.0f);
		float sine = std::sqrt(1.0f - cosine * cosine);
		vec3_t scatter_dir;

		if (sine * refraction_ratio > 1.0f || reflectance(cosine, refraction_ratio) > random_float()) {
			scatter_dir = reflect(unit_direction, hit_info.normal);
		}
		else {
			scatter_dir = refract(unit_direction, hit_info.normal, refraction_ratio);
		}

		vec3_t scatter_ray_origin = hit_info.point;
		scattered = ray_t(scatter_ray_origin, scatter_dir);
		return true;
	}

private:
	float ir;
	static float reflectance(float cosine, float ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

