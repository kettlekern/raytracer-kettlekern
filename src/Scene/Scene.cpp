#include "Scene.h"
#include "VectorString.h"
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"
#include <iostream>

using namespace std;

string Scene::cameraToString() {
	string retval = "Camera:\n";
	retval += " - Location: " + vec3ToString(camera.location) + "\n";
	retval += " - Up: " + vec3ToString(camera.up) + "\n";
	retval += " - Right: " + vec3ToString(camera.right) + "\n";
	retval += " - Look at: " + vec3ToString(camera.lookat) + "\n";
	return retval;
}

string Scene::lightsToString() {
	string retval = lights.size() + " light(s)\n";
	int i = 0;
	for (Light* light : lights) {
		retval += "\nLight[" + to_string(i++) + "]:\n" + light->toString();
	}
	return retval;
}

string Scene::objectsToString() {
	string retval = objects.size() + " objects(s)\n";
	int i = 0;
	for (Object* object : objects) {
		retval += "\nObject[" + to_string(i++) + "]:\n" + object->toString();
	}
	return retval;
}

void Scene::printScene() {
	string retval = cameraToString();
	retval += "\n---\n\n";
	retval += lightsToString();
	retval += "\n---\n\n";
	retval += objectsToString();
	cout << retval;
}