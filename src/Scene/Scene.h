#pragma once
#ifndef __SCENE_H_
#define __SCENE_H_

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

struct Light {
	glm::vec3 location;
	glm::vec3 color;

	std::string toString();
};

struct Camera {
	glm::vec3 location;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 lookat;

	std::string toString();
};

class Scene {
protected:
	std::vector<Light> lights;
	Camera camera;
	std::vector<Object> objects;

public:
	void printScene();

};


#endif
