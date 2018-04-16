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
	virtual Hit collide(Ray* ray) = 0;
};

class Sphere : public Object {
protected:
	float radius;
	glm::vec3 center;
public:
	Sphere(glm::vec3 color, Material mat, glm::vec3 center, float radius) : Object(color, mat), center(center), radius(radius) { name = "Sphere"; }
	Sphere(glm::vec4 color, Material mat, glm::vec3 center, float radius) : Object(color, mat), center(center), radius(radius) { name = "Sphere"; }

	float getRadius() {
		return radius;
	}

	glm::vec3 getCenter() {
		return center;
	}

	std::string toStringLocal();
	Hit collide(Ray* ray);
};

class Plane : public Object {
protected:
	float distance;
	glm::vec3 normal;

public:
	Plane(glm::vec3 color, Material mat, glm::vec3 normal, float distance) : Object(color, mat), normal(normal), distance(distance) { name = "Plane"; }
	Plane(glm::vec4 color, Material mat, glm::vec3 normal, float distance) : Object(color, mat), normal(normal), distance(distance) { name = "Plane"; }

	float getDistance() {
		return distance;
	}

	glm::vec3 getNormal() {
		return normal;
	}

	std::string toStringLocal();
	Hit collide(Ray* ray);
};

#endif
