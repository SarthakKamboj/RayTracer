#pragma once

#include <memory>
#include <vector>
#include "hit.h"

class hittable_manager_t : public hittable {
public:
	void add(const std::shared_ptr<hittable>& ptr) {
		objects.push_back(ptr);
	}

	virtual bool hit(const ray_t& ray, float t_min, float t_max, hit_info_t& hit_info) const override;

private:
	std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_manager_t::hit(const ray_t& ray, float t_min, float t_max, hit_info_t& hit_info) const {

	bool hit = false;
	float cur_max = t_max;
	hit_info_t temp_hit_info{};

	for (const std::shared_ptr<hittable>& obj : objects) {
		bool obj_hit = obj->hit(ray, t_min, cur_max, temp_hit_info);
		if (obj_hit) {
			hit = true;
			cur_max = temp_hit_info.t;
			hit_info = temp_hit_info;
		}
	}

	return hit;
}
