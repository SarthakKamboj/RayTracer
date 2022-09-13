#pragma once

#include "vec3.h"

class camera_t {
public:
	camera_t() {
		float image_width = 800;
		float image_height = image_width / aspect_ratio;

		viewport_height = vec3_t(0.0f, 2.0f, 0.0f);
		viewport_width = vec3_t(viewport_height.y * aspect_ratio, 0, 0);

		lower_left = origin - (viewport_height / 2) - (viewport_width / 2) - point_t(0, 0, focal_len);
	}

	ray_t get_ray(float x, float y) {
		vec3_t dir = lower_left + y * viewport_height + x * viewport_width - origin;
		ray_t ray(origin, dir);
		return ray;
	}

	const float aspect_ratio = 16.0f / 9.0f;
	point_t origin = point_t(0, 0, 0);
	point_t lower_left;
	float focal_len = 1.0f;
	vec3_t viewport_height;
	vec3_t viewport_width;
};
