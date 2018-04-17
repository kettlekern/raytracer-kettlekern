
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"
#include "Ray/RayManager.h"

using namespace std;

struct ImageCoords {
	int width;
	int height;
};

struct Point {
	int x; 
	int y;
};

//Side effect of indexing rays from the top left instead of the bottom left
int flipRayPixelY(int y, int height) {
	return height - y - 1;
}

void parseRaycast(int argc, char** argv, ImageCoords & image) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
}
void parsePixelray(int argc, char** argv, ImageCoords & image, Point & point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	//Because the image is flipped on output and so rays index from the top, not the bottom
	point.y = stoi(argv[6]);
}
void parseFirstHit(int argc, char** argv, ImageCoords & image, Point & point) {
	parsePixelray(argc, argv, image, point);
}

std::string pixelrayToString(const Hit & val, Ray* ray, const Point & rayLoc, int height) {
	std::string retval;
	if (val.isHit) {
		retval += "Pixel: [" + to_string(rayLoc.x) + ", " + to_string(rayLoc.y) + "] " +
			"Ray: {" + formatted_to_string(ray->origin.x) + " " + formatted_to_string(ray->origin.y) + " " + formatted_to_string(ray->origin.z) + "} -> {" +
			formatted_to_string(ray->direction.x) + " " + formatted_to_string(ray->direction.y) + " " + formatted_to_string(ray->direction.z) + "}\n";
	}
	else {
		retval = "No Hit\n";
	}
	return retval;
}

std::string firstHitToString(const Hit & val, Ray* ray, const Point & point, int height) {
	std::string retval;
	if (val.isHit) {
		retval += "Pixel: [" + to_string(point.x) + ", " + to_string(point.y) + "] " +
			"Ray: {" + formatted_to_string(ray->origin.x) + " " + formatted_to_string(ray->origin.y) + " " + formatted_to_string(ray->origin.z) + "} -> {" +
			formatted_to_string(ray->direction.x) + " " + formatted_to_string(ray->direction.y) + " " + formatted_to_string(ray->direction.z) + "}\n";
		retval += "T = " + formatted_to_string(val.t) + "\n";
		retval += "Object Type: " + val.objType + "\n";
		retval += "Color: " + formatted_to_string(val.color.x) + " " + formatted_to_string(val.color.y) + " " + formatted_to_string(val.color.z) + "\n";
	}
	else {
		retval = "No Hit\n";
	}
	return retval;
}

int runCommand(int argc, char** argv) {
	string command = argv[1];
	string filename = argv[2];
	cout.precision(4);
	Scene* scene = parseFile(filename);
	if (scene == nullptr) {
		cout << "Could not parse file\n";
	}
	ImageCoords image;
	Point point;
	if (command == "raycast") {
		parseRaycast(argc, argv, image);
		castRays(image.width, image.height, scene);
	}
	else if (command == "sceneinfo") {
		scene->printScene();
	}
	else if (command == "pixelray") {
		parsePixelray(argc, argv, image, point);
		Ray* ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << pixelrayToString(value, ray, point, image.height);
	}
	else if (command == "firsthit") {
		parseFirstHit(argc, argv, image, point);
		Ray* ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << firstHitToString(value, ray, point, image.height);
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