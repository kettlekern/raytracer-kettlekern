#include "Plane.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

float Plane::collide(Ray ray) {
	float t = (distance - glm::dot(ray.origin, normal))/glm::dot(ray.direction, normal);
	return t;
}

void Plane::addTranslate(const glm::vec3 & translate)
{
	distance += glm::dot(transformNormal(normal, true), translate);
}

void Plane::invertModel()
{
	__super::invertModel();
	normal = transformNormal(normal);
}

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + formatted_to_string(distance) + "\n";
}