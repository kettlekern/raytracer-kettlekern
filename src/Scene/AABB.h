#pragma once
#ifndef __SCENE_AABB_H_473_
#define __SCENE_AABB_H_473_

#include <glm/glm.hpp>
#include <utility>
#include "../Ray/Ray.h"

class AABB {
protected:
	glm::vec3 min;
	glm::vec3 max;

	std::pair<float, float> getLinearIntersection(const Ray & ray, int index);


public:
	AABB() 
	{
		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(std::numeric_limits<float>::lowest());
	}
	AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
	
	glm::vec3 getMin() { return min; }
	glm::vec3 getMax() { return max; }

	void addPoint(const glm::vec3 & point);

	float collide(const Ray & ray);
	glm::vec3 getNormal(const glm::vec3 & position, const glm::vec3 & rayDirection);
};

#endif
