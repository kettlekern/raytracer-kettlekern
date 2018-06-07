#include "Plane.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

float Plane::collide(Ray ray) {
	float t;
	//Throw out values greater than or equal to 0
	if (glm::dot(ray.direction, normal) < 0.0f) {
		t = (distance - glm::dot(ray.origin, normal))/glm::dot(ray.direction, normal);
	}
	else {
		t = -1.0f;
	}
	return t;
}

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + formatted_to_string(distance) + "\n";
}