#pragma once
#ifndef __SCENE_OBJECT_H_473_
#define __SCENE_OBJECT_H_473_

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Material.h"
#include "../Ray/Ray.h"
#include "../Hit.h"
#include "../FormattedToString.h"

class Object
{
protected:
	glm::vec3 color;
	Material mat;
	std::string name;
	//returns the smallest positive root of the equation given by -b+-sqrt(b^2-4ac)/2a, or a negative number if neither is positive
	float quadraticRoot(float a, float b, float c);

public:
	Object(glm::vec3 color, Material mat) : color(color), mat(mat) {}
	Object(glm::vec4 color, Material mat) : color(glm::vec3(color)), mat(mat) {}

	glm::vec3 getColor() {
		return color;
	}

	Material getMaterial() {
		return mat;
	}

	std::string toString();
	virtual std::string toStringLocal() = 0;
	virtual glm::vec3 getNormal(glm::vec3 position) = 0;
	virtual Hit collide(Ray* ray) = 0;
};

#endif
