#include "VectorString.h"

std::string Parser::vec3ToString(const glm::vec3 & vec) {
	std::string retval = "{";
	retval += "" + formatted_to_string(vec.x) + " " + formatted_to_string(vec.y) + " " + formatted_to_string(vec.z) + "}";
	return retval;
}