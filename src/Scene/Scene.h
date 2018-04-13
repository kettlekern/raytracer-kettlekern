#pragma once
#ifndef __SCENE_H_
#define __SCENE_H_

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

struct Light {
	glm::vec3 location;
	glm::vec3 color;

	Light(glm::vec3 location, glm::vec3 color) :location(location), color(color) {}

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
	std::vector<Light*> lights;
	Camera camera;
	std::vector<Object*> objects;

public:
	Scene() {}
	void printScene();

	std::string cameraToString();

	std::string lightsToString();
	std::string objectsToString();

	void setCamera(glm::vec3 location, glm::vec3 up, glm::vec3 right, glm::vec3 lookat) {
		camera.location = location; 
		camera.up = up;
		camera.right = right;
		camera.lookat = lookat;
	}

	void addLight(glm::vec3 location, glm::vec3 color) {
		lights.push_back(new Light(location, color));
	}

	void addLight(Light* light) {
		lights.push_back(light);
	}

	void addObject(Object* object) {
		objects.push_back(object);
	}

};


#endif
