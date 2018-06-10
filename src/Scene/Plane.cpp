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
	distance += glm::dot(normal, translate);
}

void Plane::addRotate(const glm::vec3 & rotate)
{
	glm::mat4 Rotation = glm::mat4(1.f);

	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * Rotation;
	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0, 1, 0)) * Rotation;
	Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1, 0, 0)) * Rotation;

	if (Rotation != glm::mat4(1.0f)) {
		glm::vec4 temp = glm::vec4(normal, 0.0f);
		temp = temp * glm::transpose(glm::inverse(Rotation));
		normal = glm::vec3(temp.x, temp.y, temp.z);
	}
}

void Plane::addScale(const glm::vec3 & scale)
{
	distance *= glm::dot(normal, scale);
}

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + formatted_to_string(distance) + "\n";
}