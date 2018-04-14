#pragma once
#ifndef __RAY_H_
#define __RAY_H_

#include <glm/glm.hpp>
#include <vector>
#include "../Scene/Object.h"

class Ray {
public:
	Ray() {}
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction){}
	glm::vec3 origin;
	glm::vec3 direction;

	Hit collide(Scene* scene);
	Hit collideWith(Object* obj);
};




#endif