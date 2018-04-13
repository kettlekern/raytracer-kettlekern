#pragma once
#ifndef __VECTOR_STRING_H_
#define __VECTOR_STRING_H_

#include <glm/glm.hpp>
#include <string>

std::string vec3ToString(const glm::vec3 & vec) {
	std::string retval = "{";
	retval += "" + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + "}";
	return retval;
}

#endif