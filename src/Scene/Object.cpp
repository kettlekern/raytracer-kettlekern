#include "Object.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Material.h"
#include "../Ray/Ray.h"
#include "VectorString.h"

using namespace std;

string Object::toString() {
	string retval = " - Type: " + name + "\n";
	retval       += toStringLocal() +
		            " - Color: " + Parser::vec3ToString(color) + "\n" +
		            mat.toString() + "\n";
	return retval;
}

void Object::addTranslate(const glm::vec3 & translate)
{
	model = glm::translate(glm::mat4(1.0f), translate) * model;
}

void Object::addRotate(const glm::vec3 & rotate)
{
	glm::mat4 Rotation = glm::mat4(1.f);

	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * Rotation;
	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0, 1, 0)) * Rotation;
	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1, 0, 0)) * Rotation;

	model = Rotation * model;
}

void Object::addScale(const glm::vec3 & scale)
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	model = scaleMatrix * model;
}

Ray Object::transformRay(const Ray & ray) 
{
	Ray returnRay;
	returnRay.origin = glm::vec3(model * glm::vec4(ray.origin, 1.0f));
	returnRay.direction = glm::vec3(model * glm::vec4(ray.direction, 0.0f));
	returnRay.ior = ray.ior;

	return returnRay;
}

glm::vec3 Object::transformNormal(const glm::vec3 & normal)
{
	glm::mat4 normalMatrix = glm::transpose(model);
	glm::vec3 newNormal = glm::vec3(normalMatrix * glm::vec4(normal, 0.0f));
	return normalize(newNormal);
}

//This is here so planes can be translated easily
glm::vec3 Object::transformNormal(const glm::vec3 & normal, bool invertModel)
{
	glm::vec3 ret;
	if (invertModel) {
		glm::mat4 temp = model;
		Object::invertModel();
		ret = transformNormal(normal);
		model = temp;
	}
	else {
		ret = transformNormal(normal);
	}
	return ret;
}

void Object::invertModel()
{
	model = glm::inverse(model);
}

float Object::quadraticRoot(float a, float b, float c) 
{
	float first, second;
	first = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	second = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	if (first < 0) {
		return second;
	}
	else if (second < 0) {
		return first;
	}
	else if (first < second) {
		return first;
	}
	else {
		return second;
	}
}
