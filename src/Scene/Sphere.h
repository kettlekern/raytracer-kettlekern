#pragma once
#ifndef __SCENE_SPHERE_H_473_
#define __SCENE_SPHERE_H_473_

#include "Object.h"

class Sphere : public Object {
protected:
	float radius;
	glm::vec3 center;
public:
	Sphere(glm::vec3 color, Material mat, glm::vec3 center, float radius) : Object(color, mat), center(center), radius(radius) { name = "Sphere"; }
	Sphere(glm::vec4 color, Material mat, glm::vec3 center, float radius) : Object(color, mat), center(center), radius(radius) { name = "Sphere"; }

	float getRadius() {
		return radius;
	}

	glm::vec3 getCenter() {
		return center;
	}

	std::string toStringLocal();
	Hit collide(Ray* ray);
	glm::vec3 getNormal(glm::vec3 position);
};

#endif