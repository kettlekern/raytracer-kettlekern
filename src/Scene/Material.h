#pragma once
#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include <glm/glm.hpp>
#include <vector>

class Material
{
public:
	Material() { ambient = 0, diffuse = 0; }
	Material(float ambient, float diffuse) : ambient(ambient), diffuse(diffuse) {}
	float ambient;
	float diffuse;

	std::string toString();
};


#endif
