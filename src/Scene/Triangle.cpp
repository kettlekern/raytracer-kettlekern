#include "Triangle.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Ray/Ray.h"
#include "VectorString.h"

//Does the determinant math, can look here to make more efficent
float Triangle::checkDeterminant(Ray ray, float detA, glm::vec3 first, glm::vec3 second, glm::vec3 third)
{
	glm::mat3 betaMat(first, second, third);
	float detBeta = glm::determinant(betaMat);
	return detBeta / detA;
}

float Triangle::collide(Ray ray) {
	float t;
	glm::vec3 normal = getNormal(pointC, ray.direction);
	//Throw out values greater than or equal to 0, this is the same as a plane check and is here to make checks faster
	if (glm::dot(ray.direction, normal) < 0.0f) {
		//maybe calculate the subtracted vectors only once and store them
		glm::mat3 A(pointA - pointB, pointA - pointC, ray.direction);
		float detA = glm::determinant(A);
		t = checkDeterminant(ray, detA, pointA - pointB, pointA - pointC, pointA - ray.origin);
		//I think this if statement check is redundant, test later.
		if (t > 0.0f) {
			//These checks are for the triangle bounds, the limits being the cone produced by the two used sides
			float beta = checkDeterminant(ray, detA, pointA - ray.origin, pointA - pointC, ray.direction);
			if (beta < 0.0f || beta > 1.0f) {
				t = -1.0f;
			}
			else {
				float gamma = checkDeterminant(ray, detA, pointA - pointB, pointA - ray.origin, ray.direction);
				//The second clause here accounts for alpha
				if (gamma < 0.0f || gamma > 1.0f - beta) {
					t = -1.0f;
				}
			}
		}
	}
	else {
		t = -1.0f;
	}
	return t;
}

std::string Triangle::toStringLocal() {
	return " - Point A: " + Parser::vec3ToString(pointA) + "\n" +
		   " - Point B: " + Parser::vec3ToString(pointB) + "\n" +
		   " - Point C: " + Parser::vec3ToString(pointC) + "\n" + 
		   " - Normal : " + Parser::vec3ToString(getNormal(pointC, pointC)) + "\n";
}

//The argument passed in is ignored, only used for spheres
glm::vec3 Triangle::getNormal(glm::vec3 position, glm::vec3 rayDirection) {
	glm::vec3 normal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
	if (glm::dot(normal, rayDirection) > 0) {
		normal = -normal;
	}
	return normal;
}