#pragma once
#ifndef __SCENE_H_473_
#define __SCENE_H_473_

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Object.h"
#include "VectorString.h"
#include "../FormattedToString.h"

struct Light {
	glm::vec3 location;
	glm::vec3 color;

	Light(glm::vec3 location, glm::vec3 color) : location(location), color(color) {}

	std::string toString();
};

struct Camera {
	glm::vec3 location;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 lookat;
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

	std::vector<Object*> getObjects() { return objects; }

	void addLight(glm::vec3 location, glm::vec3 color) {
		lights.push_back(new Light(location, color));
	}

	void addLight(Light* light) {
		lights.push_back(light);
	}

	Camera getCamera() { return camera; }

	std::vector<Light*> getLights() { return lights; }

	void addObject(Object* object) {
		objects.push_back(object);
	}

};


#endif
