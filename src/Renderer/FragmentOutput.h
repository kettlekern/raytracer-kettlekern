#pragma once
#ifndef __FRAGMENT_OUTPUT_473_
#define __FRAGMENT_OUTPUT_473_
#include <glm/glm.hpp>
#include <vector>
#include "Fragment.h"

class FragmentOutput {
	std::vector<Fragment> fragBuffer;
	glm::vec3 color;

public:
	FragmentOutput(std::vector<Fragment> & fragBuffer) : fragBuffer(fragBuffer) {
		for (Fragment & frag : fragBuffer){
			color += frag.getColor();
		}
		color /= glm::vec3(fragBuffer.size());
	}

	glm::vec3 getColor() { return color; }
};



#endif