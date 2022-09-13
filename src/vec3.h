#pragma once
#include <cmath>

class vec3_t {
public:
	vec3_t() {}
	vec3_t(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
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


};

inline std::ostream& operator<<(std::ostream& out, const vec3_t& vec) {
	out << vec.x << " " << vec.y << " " << vec.z << std::endl;
	return out;
}

inline vec3_t operator*(float t, const vec3_t& vec) {
	return vec3_t(vec.x * t, vec.y * t, vec.z * t);
}

inline vec3_t operator*(const vec3_t& vec, float t) {
	return vec3_t(vec.x * t, vec.y * t, vec.z * t);
}

inline vec3_t operator/(vec3_t v, double t) {
	return (1 / t) * v;
}

inline double dot(const vec3_t& u, const vec3_t& v) {
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
