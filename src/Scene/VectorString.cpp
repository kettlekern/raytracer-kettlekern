#include "VectorString.h"

std::string Parser::vec3ToString(const glm::vec3 & vec) {
	std::string retval = "{";
	retval += "" + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + "}";
	return retval;
}