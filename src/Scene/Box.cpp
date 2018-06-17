#include "Box.h"


void Box::addPoint(const glm::vec3 & point)
{
	boundingBox.addPoint(point);
}

std::string Box::toStringLocal()
{
	return "";
}

float Box::collide(Ray ray)
{
	float t = 0.0f;
	Ray newRay = transformRay(ray);
	t = boundingBox.collide(newRay);
	return t;
}

glm::vec3 Box::getNormal(glm::vec3 position, glm::vec3 rayDirection)
{
	glm::vec3 objectSpacePosition = glm::vec3(model * glm::vec4(position, 1.0f));
	glm::vec3 normal = boundingBox.getNormal(objectSpacePosition, rayDirection);
	normal = transformNormal(normal);
	if (glm::dot(normal, rayDirection) > 0) {
		normal = -normal;
	}
	return normal;
}
