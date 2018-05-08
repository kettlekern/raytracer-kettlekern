#pragma once
#ifndef __SCENE_TRIANGLE_H_473_
#define __SCENE_TRIANGLE_H_473_

#include "Object.h"

class Triangle : public Object {
protected:
	glm::vec3 pointA, pointB, pointC;
	float checkDeterminant(Ray * ray, float detA, glm::vec3 first, glm::vec3 second, glm::vec3 third);
public:
	Triangle(glm::vec3 color, Material mat, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC) : Object(color, mat), pointA(pointA), pointB(pointB), pointC(pointC) { name = "Triangle"; }
	Triangle(glm::vec4 color, Material mat, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC) : Object(color, mat), pointA(pointA), pointB(pointB), pointC(pointC) { name = "Triangle"; }

	std::string toStringLocal();
	float collide(Ray* ray);
	glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection);
};

#endif
