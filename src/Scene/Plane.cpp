#include "Plane.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

float Plane::collide(Ray ray) {
	float t = (distance - glm::dot(ray.origin, normal))/glm::dot(ray.direction, normal);
	return t;
}

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + formatted_to_string(distance) + "\n";
}