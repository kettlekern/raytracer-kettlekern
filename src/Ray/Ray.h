#pragma once
#ifndef __RAY_H_473_
#define __RAY_H_473_

#include <glm/glm.hpp>
#include <vector>

class Ray {
public:
	Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}
	Ray(glm::vec3 origin, glm::vec3 direction, float ior, bool entering, int objID, Ray* fromRay) : origin(origin), direction(direction), ior(ior), entering(entering), objID(objID), fromRay(fromRay) {}
	glm::vec3 origin;
	glm::vec3 direction;
	float ior = 1.0f;
	bool entering = false;
	int objID = -1;
	Ray* fromRay = nullptr;
};




#endif