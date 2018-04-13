#pragma once
#ifndef __SCENE_OBJECT_H_
#define __SCENE_OBJECT_H_

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Material.h"

class Object
{
protected:
	glm::vec3 color;
	Material mat;
	std::string name;

public:
	Object(glm::vec3 color, Material mat) : color(color), mat(mat) {}
	Object(glm::vec4 color, Material mat) : color(glm::vec3(color)), mat(mat) {}

	glm::vec3 getColor() {
		return color;
	}

	Material getMaterial() {
		return mat;
	}

	std::string vec3ToString(glm::vec3 vec);

	std::string toString();
	virtual std::string toStringLocal() = 0;
};

class Sphere : public Object {
protected:
	float distance;
	glm::vec3 center;
public:
	Sphere(glm::vec3 color, Material mat, glm::vec3 center, float distance) : Object(color, mat), center(center), distance(distance) { name = "Sphere"; }

	float getRadius() {
		return distance;
	}

	glm::vec3 getCenter() {
		return center;
	}

	std::string toStringLocal();
};

class Plane : public Object {
protected:
	float distance;
	glm::vec3 normal;
public:
	Plane(glm::vec3 color, Material mat, glm::vec3 normal, float distance) : Object(color, mat), normal(normal), distance(distance) { name = "Plane"; }

	float getDistance() {
		return distance;
	}

	glm::vec3 getNormal() {
		return normal;
	}

	std::string toStringLocal();
};


#endif
