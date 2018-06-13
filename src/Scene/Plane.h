#pragma once
#ifndef __SCENE_PLANE_H_473_
#define __SCENE_PLANE_H_473_

#include "Object.h"

class Plane : public Object {
protected:
	float distance;
	glm::vec3 normal;

public:
	Plane(glm::vec3 color, Material mat, int id, glm::vec3 normal, float distance) : Object(color, mat, id), normal(normal), distance(distance) { name = "Plane"; }
	Plane(glm::vec4 color, Material mat, int id, glm::vec3 normal, float distance) : Object(color, mat, id), normal(normal), distance(distance) { name = "Plane"; }

	float getDistance() { return distance; }

	std::string toStringLocal();
	float collide(Ray ray);

	void addTranslate(const glm::vec3 & translate);
	void invertModel();

	glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection) {
		return glm::dot(normal, rayDirection) < 0 ? normal : -normal;
	}
};

#endif
