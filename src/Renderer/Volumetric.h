#pragma once
#ifndef __VOLUMETRIC_473_
#define __VOLUMETRIC_473_
#include <glm/glm.hpp>
#include "Simplex Noise/OpenSimplexNoise.hh"

#define FOG_WEIGHT 0.1f
#define FOG_SAMPLES 20

class Volumetric {

	//The amount of light affected by the participating media per unit length.
	//Ideally this would be a random function that is easy to take an integral of, but for now is just one constant value
	OSN::Noise<4>* noise;
	glm::vec3 color;
	float fogWeight = FOG_WEIGHT;
	float time = 0.0f;
	int fogSamples = FOG_SAMPLES;
	float calcDensityFromTo(const glm::vec3 & from, const glm::vec3 & to);
	float evalPoint(glm::vec3 point);
	//float calcDensityFromTo(glm::vec3 from, glm::vec3 to) { return 0.01f; }
	float capAmount(float amount) { return amount > 1.0f ? 1.0f : amount < 0.0f ? 0.0f : amount; }

public:
	Volumetric(glm::vec3 color, OSN::Noise<4>* noise) : color(color), noise(noise) {}

	//From and to are two points in the world that are in fog
	float calcFogAmount(float timeInFog, float density) { return capAmount(timeInFog * density); }
	float calcDensity(glm::vec3 from, glm::vec3 direction, float t) { return calcDensityFromTo(from, t * direction + from); }
	float fogGathered(glm::vec3 from, glm::vec3 to);
	glm::vec3 fogColorGathered(float fogAmount);
	glm::vec3 getColor() { return color; }
	void setTime(float t) { time = t; }
	void setWeight(float density) { fogWeight = density; }

};



#endif