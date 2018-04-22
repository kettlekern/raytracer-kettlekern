#include "Plane.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

Hit Plane::collide(Ray* ray) {
	Hit retval;
	//Throw out values greater than or equal to 0
	if (glm::dot(ray->direction, normal) < 0) {
		retval.t = (distance - glm::dot(ray->origin, normal))/glm::dot(ray->direction, normal);
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

std::string Plane::toStringLocal() {
	return " - Normal: " + Parser::vec3ToString(normal) + "\n" +
		   " - Distance: " + formatted_to_string(distance) + "\n";
}