#pragma once

#include "vec3.h"

class camera_t {
public:
	camera_t(float aspect_ratio, float vfov, point_t look_from, point_t look_at) {
		// float image_width = 800;
		// float image_height = image_width / aspect_ratio;

		vec3_t w = unit_vector(look_from - look_at);
		vec3_t v_up(0, 1.0f, 0);
		vec3_t right = unit_vector(cross(v_up, w));
		vec3_t up = unit_vector(cross(w, right));

		float vfov_radians = degrees_to_radians(vfov);
		float v_height = 2.0f * tan(vfov_radians / 2.0f);
		float v_width = v_height * aspect_ratio;

		// viewport_height = vec3_t(0.0f, v_height, 0.0f);
		// viewport_width = vec3_t(v_width, 0, 0);

		horizontal = right * v_width;
		vertical = up * v_height;

		horizontal = vec3_t(v_width, 0, 0);
		vertical = vec3_t(0, v_height, 0);

		// lower_left = origin - (viewport_height / 2) - (viewport_width / 2) - point_t(0, 0, focal_len);
		lower_left = origin - (horizontal / 2) - (vertical / 2) - point_t(0, 0, focal_len);
	}

	ray_t get_ray(float s, float t) {
		// vec3_t dir = lower_left + s * viewport_width + t * viewport_height - origin;
		vec3_t dir = lower_left + s * horizontal + t * vertical - origin;
		ray_t ray(origin, dir);
		return ray;
	}

	// const float aspect_ratio = 16.0f / 9.0f;
	point_t origin = point_t(0, 0, 0);
	point_t lower_left;
	float focal_len = 1.0f;
	// vec3_t viewport_height, viewport_width;
	vec3_t horizontal, vertical;
};
