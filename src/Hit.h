#pragma once
#ifndef __HIT_H_473_
#define __HIT_H_473_

#include <string.h>
#include "Scene/Material.h"

struct Hit {
	bool isHit;
	Material* mat;
	std::string objType;
	int t;
};

#endif