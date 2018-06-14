#pragma once
#ifndef __VOLUMETRIC_473_
#define __VOLUMETRIC_473_
#include <glm/glm.hpp>
#include "../Scene/Object.h"
/*
#ifndef __SIMPLEXNOISE__
#define __SIMPLEXNOISE__
#include "Simplex Noise/OpenSimplexNoise.hh"
#endif // !__SIMPlEXNOISE__
*/
class Volumetric {

	//The amount of light affected by the participating media per unit length.
	//Ideally this would be a random function that is easy to take an integral of, but for now is just one constant value
	//OSN::Noise<3> noise;
	glm::vec3 color;
	//float calcDensityFromTo(glm::vec3 from, glm::vec3 to) { return noise.eval(from.x, from.y, from.z); }
	float calcDensityFromTo(glm::vec3 from, glm::vec3 to) { return 0.01f; }
	float capAmount(float amount) { return amount > 1.0f ? 1.0f : amount < 0.0f ? 0.0f : amount; }

public:
	Volumetric(glm::vec3 color) : color(color) {}

	//From and to are two points in the world that are in fog
	float calcFogAmount(float timeInFog, float density) { return capAmount(timeInFog * density); }
	float calcDensity(glm::vec3 from, glm::vec3 direction, float t) { return calcDensityFromTo(from, t * direction + from); }
	glm::vec3 fogColorGathered(glm::vec3 from, glm::vec3 to);
	glm::vec3 fogColorGathered(float fogAmount);
	glm::vec3 getColor() { return color; }

};



#endif