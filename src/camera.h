#pragma once

#include "vec3.h"

class camera_t {
public:
	camera_t(float aspect_ratio, float vfov, point_t look_from, point_t look_at, float aperture, float focus_dist) {
		w = unit_vector(look_from - look_at);
		vec3_t v_up(0, 1.0f, 0);
		right = unit_vector(cross(v_up, w));
		up = cross(w, right);

		float vfov_radians = degrees_to_radians(vfov);
		float viewport_height = 2.0f * tan(vfov_radians / 2.0f);
		float viewport_width = viewport_height * aspect_ratio;

		horizontal = focus_dist * right * viewport_width;
		vertical = focus_dist * up * viewport_height;

		origin = look_from;
		lower_left = origin - (horizontal / 2) - (vertical / 2) - w * focal_len * focus_dist;

		lens_radius = aperture / 2.0f;
	}

	ray_t get_ray(float s, float t) {

		vec3_t rd = lens_radius * random_in_unit_disk();
		vec3_t offset = rd.x * right + rd.y * up;

		point_t ray_orig = origin + offset;
		vec3_t dir = lower_left + s * horizontal + t * vertical - ray_orig;
		ray_t ray(ray_orig, dir);
		return ray;
	}

	point_t origin;
	point_t lower_left;
	float focal_len = 1.0f;
	float lens_radius;
	vec3_t horizontal, vertical;
	vec3_t right, up, w;
};
