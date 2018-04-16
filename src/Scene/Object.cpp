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

float Object::quadraticRoot(float a, float b, float c) {
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

Hit Plane::collide(Ray* ray) {
	Hit retval;
	if (glm::dot(ray->direction, normal) > 0) {
		retval.t = (-distance - glm::dot(ray->origin, normal))/glm::dot(ray->direction, normal);
		if (retval.t >= 0) {
			retval.isHit = true;
			retval.mat = &(this->mat);
			retval.objType = name;
			retval.color = color;
		}
	}
	else {
		retval.isHit = false;
	}
	return retval;
}

std::string Sphere::toStringLocal() {
	return " - Center: " + Parser::vec3ToString(center) + "\n" +
		   " - Radius: " + to_string(radius) + "\n";
}

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + to_string(distance) + "\n";
}