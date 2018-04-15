
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"
#include "Ray/RayManager.h"


using namespace std;

struct Image {
	int width;
	int height;
};

struct Point {
	int x; 
	int y;
};

void parseRaycast(int argc, char** argv, Image & image) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
}
void parsePixelray(int argc, char** argv, Image & image, Point & point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	point.y = stoi(argv[6]);
}
void parseFirstHit(int argc, char** argv, Image & image, Point & point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	point.y = stoi(argv[6]);
}

std::string pixelrayToString(const Hit & val, Ray* ray, const Image & image) {
	std::string retval;
	if (val.isHit) {
		retval += "Pixel: [" + to_string(image.width) + ", " + to_string(image.height) + "] " +
			"Ray: {" + to_string(ray->origin.x) + " " + to_string(ray->origin.y) + " " + to_string(ray->origin.z) + "} -> {" +
			to_string(ray->direction.x) + " " + to_string(ray->direction.y) + " " + to_string(ray->direction.z) + "}\n";
	}
	else {
		retval = "No Hit\n";
	}
	return retval;
}

std::string firstHitToString(const Hit & val, Ray* ray, const Image & image) {
	std::string retval;
	if (val.isHit) {
		retval += "Pixel: [" + to_string(image.width) + ", " + to_string(image.height) + "] " +
			"Ray: {" + to_string(ray->origin.x) + " " + to_string(ray->origin.y) + " " + to_string(ray->origin.z) + "} -> {" +
			to_string(ray->direction.x) + " " + to_string(ray->direction.y) + " " + to_string(ray->direction.z) + "}\n";
		retval += "T = " + to_string(val.t) + "\n";
		retval += "Object Type: " + val.objType + "\n";
		retval += "Color: " + to_string(val.color.x) + " " + to_string(val.color.y) + " " + to_string(val.color.z) + "\n";
	}
	else {
		retval = "No Hit\n";
	}
	return retval;
}

int runCommand(int argc, char** argv) {
	string command = argv[1];
	string filename = argv[2];
	Scene* scene = parseFile(filename);
	if (scene == nullptr) {
		cout << "Could not parse file\n";
	}
	Image image;
	Point point;
	if (command == "raycast") {
		parseRaycast(argc, argv, image);
	}
	else if (command == "sceneinfo") {
		scene->printScene();
	}
	else if (command == "pixelray") {
		parsePixelray(argc, argv, image, point);
		Ray* ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << pixelrayToString(value, ray, image);
	}
	else if (command == "firsthit") {
		parseFirstHit(argc, argv, image, point);
		Ray* ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << firstHitToString(value, ray, image);
	}
	else {
		cout << "Unknown command: " << command << ", exiting.\n";
		return -1;
	}
	return 0;
}

int runArgs(int argc, char** argv) {
	if (argc < 2) {
		cout << "Invalid call: no command given\n";
		return -1;
	}
	return runCommand(argc, argv);
}

int main(int argc, char** argv) {
	return runArgs(argc, argv);
}