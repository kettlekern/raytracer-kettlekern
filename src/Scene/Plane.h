#pragma once
#ifndef __SCENE_PLANE_H_473_
#define __SCENE_PLANE_H_473_

#include "Object.h"

class Plane : public Object {
protected:
	float distance;
	glm::vec3 normal;

public:
	Plane(glm::vec3 color, Material mat, glm::vec3 normal, float distance) : Object(color, mat), normal(normal), distance(distance) { name = "Plane"; }
	Plane(glm::vec4 color, Material mat, glm::vec3 normal, float distance) : Object(color, mat), normal(normal), distance(distance) { name = "Plane"; }

	float getDistance() {
		return distance;
	}

	glm::vec3 getNormal() {
		return normal;
	}

	std::string toStringLocal();
	Hit collide(Ray* ray);
	glm::vec3 getNormal(glm::vec3 position) {
		return normal;
	}
};

#endif