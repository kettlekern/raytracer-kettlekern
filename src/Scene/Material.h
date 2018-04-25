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
	float ambient;
	float diffuse;
	float specular;
	float roughness;
	float metallic;
	float ior;

	Material() { ambient = 0, diffuse = 0; specular = 0.0f; roughness = 0.0f; ior = 1.0f; metallic = 0.0f; }
	
	std::string toString();
};


#endif
