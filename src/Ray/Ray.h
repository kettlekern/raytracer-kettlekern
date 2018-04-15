#pragma once
#ifndef __RAY_H_473_
#define __RAY_H_473_

#include <glm/glm.hpp>
#include <vector>

class Ray {
public:
	Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction){}
	glm::vec3 origin;
	glm::vec3 direction;
};




#endif