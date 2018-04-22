#include "Sphere.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

Hit Sphere::collide(Ray* ray) {
	Hit retval;
	glm::vec3 PominC = (ray->origin - center);
	float a = glm::dot(ray->direction, ray->direction);
	float b = 2 * glm::dot(ray->direction, PominC);
	float c = glm::dot(PominC, PominC) - radius * radius;
	float root = quadraticRoot(a, b, c);
	if (root >= 0) {
		retval.isHit = true;
		retval.mat = &(this->mat);
		retval.objType = name;
		retval.color = color;
		retval.t = root;
	}
	else {
		retval.isHit = false;
	}
	return retval;
}

std::string Sphere::toStringLocal() {
	return " - Center: " + Parser::vec3ToString(center) + "\n" +
		   " - Radius: " + formatted_to_string(radius) + "\n";
}

glm::vec3 Sphere::getNormal(glm::vec3 position) {
	return glm::normalize(position - center);
}