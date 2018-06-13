#include "Cone.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

float Cone::collide(Ray ray) {
	ray = transformRay(ray);
	glm::vec3 PominC = (ray.origin - origin);
	glm::vec3 minPominC = PominC;
	minPominC.z = -minPominC.z;
	glm::vec3 minDir = ray.direction;
	minDir.z = -minDir.z;
	//Because the dot product of a normalized vector with itself is 1, we can simplify a to 1
	float a = glm::dot(ray.direction, minDir);
	float b = 2 * glm::dot(ray.direction, minPominC);
	float c = glm::dot(PominC, minPominC);
	float disc = b * b - 4 * a * c;
	//When this value is negative, the ray is intersecting the disc part at the bottom of the cone
	if (disc > 0) {
		return quadraticRoot(a, b, c);
	}
	else {
		return -1.f;//intersectDisc(ray);
	}
}

std::string Cone::toStringLocal() {
	return " - Origin: " + Parser::vec3ToString(origin) + "\n" +
		   " - Radius: " + formatted_to_string(radius) + "\n";
}

glm::vec3 Cone::getNormal(glm::vec3 position, glm::vec3 rayDirection) {
	glm::vec3 objectSpacePosition = glm::vec3(model * glm::vec4(position, 1.0f));

	glm::vec3 normal = (objectSpacePosition - origin) * height / radius;
	if (height > 0.0f) {
		normal.y = radius / height;
	}
	else {
		normal.y = 1.0f;
	}

	normal = transformNormal(normal);
	if (glm::dot(normal, rayDirection) > 0) {
		normal = -normal;
	}
	return normal;
}