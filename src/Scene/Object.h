#pragma once
#ifndef __SCENE_OBJECT_H_473_
#define __SCENE_OBJECT_H_473_

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include "Material.h"
#include "../Ray/Ray.h"
#include "../FormattedToString.h"

class Object
{
protected:
	glm::vec3 color;
	Material mat;
	std::string name;
	glm::mat4 normalMatrix = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	int id;
	bool foggy;
	//returns the smallest positive root of the equation given by -b+-sqrt(b^2-4ac)/2a, or a negative number if neither is positive
	float quadraticRoot(float a, float b, float c);

public:
	Object(glm::vec3 color, Material mat, int id) : color(color), mat(mat), id(id) {}
	Object(glm::vec4 color, Material mat, int id) : color(glm::vec3(color)), mat(mat), id(id) { mat.refraction = color.a; }

	glm::vec3 getColor() { return color; }
	std::string getName() { return name; }
	int getID() { return id; }
	bool isFoggy() { return foggy; }
	Material getMaterial() { return mat; }

	void setMaterial(Material mat) { this->mat = mat; }
	void setColor(glm::vec4 color) { this->color = color; }
	void activateFog() { foggy = true; }

	virtual void addTranslate(const glm::vec3 & translate);
	virtual void addRotate(const glm::vec3 & rotate);
	virtual void addScale(const glm::vec3 & scale);
	Ray transformRay(const Ray & ray);
	virtual void invertModel();
	glm::vec3 transformNormal(const glm::vec3 & normal);
	glm::vec3 transformNormal(const glm::vec3 & normal, bool invertModel);

	std::string toString();
	virtual std::string toStringLocal() = 0;
	//This returns a normalized, positive(normal facing the incoming ray), transformed normal vector
	virtual glm::vec3 getNormal(glm::vec3 position, glm::vec3 rayDirection) = 0;
	//Instead of returning a hit object, return a float and collect the rest of the information in the calling function
	virtual float collide(Ray ray) = 0;
};

#endif
