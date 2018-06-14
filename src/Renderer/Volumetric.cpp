#include "Volumetric.h"

float Volumetric::calcDensityFromTo(const glm::vec3 & from, const glm::vec3 & to)
{
	float total = 0.0f;
	glm::vec3 point = from;
	glm::vec3 direction = (from - to) / ((float)FOG_SAMPLES);
	for (int i = 0; i < FOG_SAMPLES; i++) {
		total += evalPoint(point);
		point += direction;
	}
	return total / FOG_SAMPLES;
}

float Volumetric::evalPoint(glm::vec3 point)
{
	float val = noise->eval(point.x, point.y, point.z);
	return ((val + 1) / 2 )* FOG_WEIGHT;
}

glm::vec3 Volumetric::fogColorGathered(glm::vec3 from, glm::vec3 to)
{
	glm::vec3 direction = from - to;
	//Get the distance the ray travels in fog
	float t = direction.length();
	//Normalize
	direction /= t;
	return color * calcFogAmount(t, calcDensityFromTo(from, to));
}



glm::vec3 Volumetric::fogColorGathered(float fogAmount)
{
	return color * fogAmount;
}