#include "Volumetric.h"

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