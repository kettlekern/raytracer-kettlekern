#pragma once
#ifndef __SCENE_CONE_H_473_
#define __SCENE_CONE_H_473_

#include "Object.h"

//By default, cones start facing down 
class Cone : public Object {
protected:
	//This radius is given by how large the projected circle is at the bottom of the cone
	float radius;
	float height;
	//The origin is in the base circle of the cone
	glm::vec3 origin;

public:
	Cone(glm::vec3 color, Material mat, int id, glm::vec3 origin, float radius, float height) : Object(color, mat, id), origin(origin), radius(radius), height(height) { name = "Cone"; }
	Cone(glm::vec4 color, Material mat, int id, glm::vec3 origin, float radius, float height) : Object(color, mat, id), origin(origin), radius(radius), height(height) { name = "Cone"; }

	float getRadius() { return radius; }
	glm::vec3 getOrigin() { return origin; }

	std::string toStringLocal();
	float collide(Ray ray);
	glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection);
};

#endif
