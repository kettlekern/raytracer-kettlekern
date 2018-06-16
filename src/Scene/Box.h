#pragma once
#ifndef __SCENE_BOX_H_473_
#define __SCENE_BOX_H_473_

#include <glm/glm.hpp>
#include <utility>
#include "AABB.h"
#include "Object.h"
#include "../Ray/Ray.h"

class Box : public Object {
protected:
	AABB boundingBox;


public:
	Box(glm::vec3 color, Material mat, int id, glm::vec3 min, glm::vec3 max) : Object(color, mat, id) { boundingBox = AABB(min, max); }
	Box(glm::vec4 color, Material mat, int id, glm::vec3 min, glm::vec3 max) : Object(color, mat, id) { boundingBox = AABB(min, max); }
	

	void addPoint(const glm::vec3 & point);

	std::string toStringLocal();
	float collide(Ray ray);
	glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection);
};

#endif
