#pragma once
#ifndef __VECTOR_STRING_H_
#define __VECTOR_STRING_H_

#include <glm/glm.hpp>
#include <string>

std::string vec3ToString(const glm::vec3 & vec) {
	std::string retval = "{";
	retval += "" + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "}";
	return retval;
}

#endif