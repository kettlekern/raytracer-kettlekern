#pragma once
#ifndef __BUFFER_473_
#define __BUFFER_473_
#include <glm/glm.hpp>
#include <vector>
#include "Fragment.h"

class Buffer {
	std::vector<std::vector<Fragment *> *>* fragBuffer;
	int width, height;
};



#endif