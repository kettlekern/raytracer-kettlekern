#pragma once
#ifndef __MATERIAL_H_473_
#define __MATERIAL_H_473_

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../FormattedToString.h"

class Material
{
public:
	Material() { ambient = 0, diffuse = 0; }
	Material(float ambient, float diffuse) : ambient(ambient), diffuse(diffuse) {}
	float ambient;
	float diffuse;
	float specular;
	float roughness;
	float ior;

	std::string toString();
};


#endif
