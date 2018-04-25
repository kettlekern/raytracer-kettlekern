#pragma once
#ifndef __HIT_H_473_
#define __HIT_H_473_

#include <string.h>
#include <glm/glm.hpp>
#include "Scene/Material.h"
#include "Scene/Object.h"

struct Hit {
	bool isHit;
	glm::vec3 color;
	glm::vec3 position;
	Object* obj;
	std::string objType;
	Material mat;
	float t;
};

#endif