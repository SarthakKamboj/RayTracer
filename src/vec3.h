#pragma once
#include <cmath>
#include "helper.h"

class vec3_t {
public:
	vec3_t() {}
	vec3_t(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	float x = 0, y = 0, z = 0;

	vec3_t(const vec3_t& orig) {
		x = orig.x;
		y = orig.y;
		z = orig.z;
	}

	vec3_t& operator=(const vec3_t& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	vec3_t operator-() const {
		return vec3_t(-x, -y, -z);
	}

	vec3_t& operator+=(const vec3_t& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3_t& operator-=(const vec3_t& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vec3_t& operator*=(const float& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	vec3_t& operator/=(const float& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}

	float length() const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}

	static vec3_t random() {
		return vec3_t(random_float(), random_float(), random_float());
	}
};

inline std::ostream& operator<<(std::ostream& out, const vec3_t& vec) {
	out << vec.x << " " << vec.y << " " << vec.z << std::endl;
	return out;
}

vec3_t add_vec3(const vec3_t& vec1, const vec3_t& vec2) {
	return vec3_t(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

inline vec3_t operator*(float t, const vec3_t& vec) {
	return vec3_t(vec.x * t, vec.y * t, vec.z * t);
}

inline vec3_t operator*(const vec3_t& vec, float t) {
	return vec3_t(vec.x * t, vec.y * t, vec.z * t);
}

inline vec3_t operator/(vec3_t v, float t) {
	return (1 / t) * v;
}

inline float dot(const vec3_t& u, const vec3_t& v) {
	return u.x * v.x
		+ u.y * v.y
		+ u.z * v.z;
}

inline vec3_t cross(const vec3_t& u, const vec3_t& v) {
	return vec3_t(u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x);
}

inline vec3_t unit_vector(const vec3_t& v) {
	return v / fmax(v.length(), 0.0001f);
}

inline vec3_t random_vec3(float min, float max) {
	return vec3_t(random_float(min, max), random_float(min, max), random_float(min, max));
}

inline vec3_t get_vec3_in_unit_sphere() {
	while (true) {
		vec3_t p = random_vec3(-1.0f, 1.0f);
		if (p.length() <= 1.0f) return p;
	}
}

vec3_t reflect(const vec3_t& in, const vec3_t& normal) {
	float proj = dot(in, normal);
	vec3_t normal_proj = proj * normal;
	vec3_t offset = -2.0f * normal_proj;
	return add_vec3(in, offset);
}

vec3_t refract(const vec3_t& uv, const vec3_t& n, float etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3_t r_out_perp = etai_over_etat * add_vec3(uv, cos_theta * n);
	vec3_t r_out_parallel = -sqrt(fabs(1.0 - (pow(r_out_perp.length(), 2)))) * n;
	return add_vec3(r_out_perp, r_out_parallel);
}

vec3_t random_unit_vector() {
	return unit_vector(get_vec3_in_unit_sphere());
}

vec3_t random_in_hemisphere(vec3_t normal) {
	vec3_t random_dir = random_unit_vector();
	if (dot(normal, random_dir) > 0) {
		return random_dir;
	}
	return -random_dir;
}

vec3_t random_in_unit_disk() {
	while (true) {
		auto p = vec3_t(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0);
		if (pow(p.length(), 2) >= 1) continue;
		return p;
	}
}

inline vec3_t operator+(const vec3_t& u, const vec3_t& v) {
	return vec3_t(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3_t operator-(const vec3_t& u, const vec3_t& v) {
	return vec3_t(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3_t operator*(const vec3_t& u, const vec3_t& v) {
	return vec3_t(u.x * v.x, u.y * v.y, u.z * v.z);
}

using point_t = vec3_t;
using color_t = vec3_t;
