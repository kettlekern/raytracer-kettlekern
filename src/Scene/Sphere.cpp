#include "Sphere.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

float Sphere::collide(Ray ray) {
	ray = transformRay(ray);
	glm::vec3 PominC = (ray.origin - center);
	//Because the dot product of a normalized vector with itself is 1, we can simplify a to 1
	float a = glm::dot(ray.direction, ray.direction);
	float b = 2 * glm::dot(ray.direction, PominC);
	float c = glm::dot(PominC, PominC) - radius * radius;
	return quadraticRoot(a, b, c);
}


Ray Sphere::transformRay(const Ray & ray)
{
	Ray returnRay;
	returnRay.origin = glm::vec3(model * glm::vec4(ray.origin, 0.0f));
	returnRay.direction = glm::vec3(model * glm::vec4(ray.direction, 0.0f));
	returnRay.ior = ray.ior;

	return returnRay;
}

void Sphere::addScale(const glm::vec3 & scale)
{
	//If it is a uniform scale, use this optimization
	if (scale.x == scale.y && scale.y == scale.z) {
		radius *= scale.x;
	}
	else {
		Object::addScale(scale);
	}
}

void Sphere::invertModel()
{
	center = glm::vec3(model * glm::vec4(center, 1.0f));
	model = glm::inverse(model);
}

std::string Sphere::toStringLocal() {
	return " - Center: " + Parser::vec3ToString(center) + "\n" +
		   " - Radius: " + formatted_to_string(radius) + "\n";
}

glm::vec3 Sphere::getNormal(glm::vec3 position, glm::vec3 rayDirection) {
	glm::vec3 objectSpacePosition = glm::vec3(model * glm::vec4(position, 1.0f));
	glm::vec3 normal = objectSpacePosition - center;
	normal = transformNormal(normal);
	if (glm::dot(normal, rayDirection) > 0) {
		normal = -normal;
	}
	return normal;
}