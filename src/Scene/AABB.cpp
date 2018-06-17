#include "AABB.h"
#include <algorithm>

#include <glm/gtc/epsilon.hpp>

//Return a pair (min, max) of the minimum and maximum t values for intersection on the axis given by index
std::pair<float, float> AABB::getLinearIntersection(const Ray & ray, int index)
{
	std::pair<float, float> retVal;
	retVal.first = std::numeric_limits<float>::max();
	retVal.second = std::numeric_limits<float>::lowest();
	//If the vector does not move on the given axis, check to see if the point is in the bounds of the box already
	if (ray.direction[index] == 0) {
		//If the point is not in bounds, send back a negative time to indicate no hit
		if (ray.origin[index] < min[index] || ray.origin[index] > max[index]) {
			retVal.first = -1;
			retVal.second = -1;
		}
	}
	else {
		//If the point is in bounds, calculate when the ray hits the box on the given axis
		retVal.first = (min[index] - ray.origin[index]) / ray.direction[index];
		retVal.second = (max[index] - ray.origin[index]) / ray.direction[index];
		if (retVal.first > retVal.second) {
			float temp = retVal.first;
			retVal.first = retVal.second;
			retVal.second = temp;
		}
	}
	return retVal;
}

void AABB::addPoint(const glm::vec3 & point)
{
	//If the new point contains any values that are smaller than the current minimum, update it
	min.x = std::min(min.x, point.x);
	min.y = std::min(min.y, point.y);
	min.z = std::min(min.z, point.z);

	//If the new point contains any values that are larger than the current maximum, update it
	max.x = std::max(max.x, point.x);
	max.y = std::max(max.y, point.y);
	max.z = std::max(max.z, point.z);
}

float AABB::collide(const Ray & ray)
{
	float t = 0.0f;
	float largestMin = std::numeric_limits<float>::lowest();
	float smallestMax = std::numeric_limits<float>::max();
	//For each dimension you are in, calculate the t values of linear intersections on that axis
	for (int i = 0; i < min.length(); ++i) {
		auto times = getLinearIntersection(ray, i);
		largestMin = std::max(times.first, largestMin);
		smallestMax = std::min(times.second, smallestMax);
	}
	//If the ray does not intersect the object
	if (largestMin > smallestMax || smallestMax < 0) {
		t = -1.0f;
	}
	//If we are inside the object
	else if (largestMin > 0) {
		t = largestMin;
	}
	//If the ray intersects the object and we are outside of it
	else {
		t = smallestMax;
	}

	return t;
}

glm::vec3 AABB::getNormal(const glm::vec3 & position, const glm::vec3 & rayDirection)
{
	glm::vec3 normal;
	float eps = 0.001f;
	//Determins the face the given point is on, then return the normal for that face
	if (glm::epsilonEqual(position.x, min.x, eps)) {
		normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (glm::epsilonEqual(position.x, max.x, eps)) {
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (glm::epsilonEqual(position.y, min.y, eps)) {
		normal = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	else if (glm::epsilonEqual(position.y, max.y, eps)) {
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (glm::epsilonEqual(position.z, min.z, eps)) {
		normal = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (glm::epsilonEqual(position.z, max.z, eps)) {
		normal = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
	return normal;
}
