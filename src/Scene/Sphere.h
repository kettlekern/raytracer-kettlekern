#pragma once
#ifndef __SCENE_SPHERE_H_473_
#define __SCENE_SPHERE_H_473_

#include "Object.h"

class Sphere : public Object {
protected:
	float radius;
	glm::vec3 center;

public:
	Sphere(glm::vec3 color, Material mat, int id, glm::vec3 center,  float radius) : Object(color, mat, id), center(center), radius(radius) { name = "Sphere"; }
	Sphere(glm::vec4 color, Material mat, int id, glm::vec3 center,  float radius) : Object(color, mat, id), center(center), radius(radius) { name = "Sphere"; }

	float getRadius() { return radius; }
	glm::vec3 getCenter() { return center; }

	void addScale(const glm::vec3 & scale);

	std::string toStringLocal();
	float collide(Ray ray);
	glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection);
};

#endif
