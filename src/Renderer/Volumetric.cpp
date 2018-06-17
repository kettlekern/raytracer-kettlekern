#include "Volumetric.h"

float Volumetric::calcDensityFromTo(const glm::vec3 & from, const glm::vec3 & to)
{
	float total = 0.0f;
	glm::vec3 point = from;
	glm::vec3 direction = (from - to) / ((float)fogSamples);
	for (int i = 0; i < fogSamples; i++) {
		total += evalPoint(point);
		point += direction;
	}
	return total / fogSamples;
}

float Volumetric::evalPoint(glm::vec3 point)
{
	float val = noise->eval(point.x, point.y, point.z, time);
	return ((val + 1) / 2 ) * fogWeight;
}

float Volumetric::fogGathered(glm::vec3 from, glm::vec3 to)
{
	glm::vec3 direction = from - to;
	//Get the distance the ray travels in fog
	float t = length(from - to);
	return calcFogAmount(t, calcDensityFromTo(from, to));
}



glm::vec3 Volumetric::fogColorGathered(float fogAmount)
{
	return color * fogAmount;
}