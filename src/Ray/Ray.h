#pragma once
#ifndef __RAY_H_473_
#define __RAY_H_473_

#include <glm/glm.hpp>
#include <vector>

class Ray {
public:
	Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}
	Ray(glm::vec3 origin, glm::vec3 direction, float ior, bool reflection, int objID, Ray* fromRay) : origin(origin), direction(direction), ior(ior), reflection(reflection), objID(objID), fromRay(fromRay) {}
	glm::vec3 origin;
	glm::vec3 direction;
	float ior = 1.0f;
	bool reflection = false;
	bool entering = true;
	int objID = -1;
	Ray* fromRay = nullptr;
};




#endif