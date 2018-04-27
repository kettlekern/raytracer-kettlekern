
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"
#include "Ray/RayManager.h"
#include "Renderer/Fragment.h"

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

void parseRender(int argc, char** argv, ImageCoords & image, bool* isAltBRDF) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	string str = "-altbrdf";
	if (argc > 5) {
		if (str.compare(argv[5]) == 0) {
			*isAltBRDF = true;
		}
	}
}

void parsePixelray(int argc, char** argv, ImageCoords & image, Point & point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	point.y = stoi(argv[6]);
}

void parsePixelcolor(int argc, char** argv, ImageCoords & image, Point & point, bool* isAltBRDF) {
	parsePixelray(argc, argv, image, point);
	string str = "-altbrdf";
	if (argc > 7) {
		if (str.compare(argv[7]) == 0) {
			*isAltBRDF = true;
		}
	}
}

void parseFirstHit(int argc, char** argv, ImageCoords & image, Point & point) {
	parsePixelray(argc, argv, image, point);
}

std::string pixelrayToString(const Hit & val, Ray* ray, const Point & rayLoc, int height) {
	std::string retval;
	retval += "Pixel: [" + to_string(rayLoc.x) + ", " + to_string(rayLoc.y) + "] " +
			"Ray: {" + formatted_to_string(ray->origin.x) + " " + formatted_to_string(ray->origin.y) + " " + formatted_to_string(ray->origin.z) + "} -> {" +
			formatted_to_string(ray->direction.x) + " " + formatted_to_string(ray->direction.y) + " " + formatted_to_string(ray->direction.z) + "}\n";
	return retval;
}

std::string pixelcolorToString(const Hit & val, Ray* ray, Scene* scene, const Point & rayLoc, int height, bool isAltBRDF) {
	std::string retval;
	std::string BRDFType = isAltBRDF ? "Cook-Torrance" : "Blinn-Phong";
	Fragment frag(val, scene);
	frag.colorFrag(scene, isAltBRDF ? COOK_TORRANCE : BLINN_PHONG);
	retval += pixelrayToString(val, ray, rayLoc, height);
	retval += "T = " + formatted_to_string(val.t) + "\n";
	retval += "Object Type: " + val.objType + "\n";
	retval += "BRDF: " + BRDFType + "\n";
	//TODO: update this and pass in a fragment instead
	retval += "Color: (" + to_string((int)(frag.getColor().r * 255 + 0.5f)) + ", " + to_string((int)(frag.getColor().g * 255 + 0.5f)) + ", " + to_string((int)(frag.getColor().b * 255 + 0.5f)) + ")\n";
	return retval;
}

std::string firstHitToString(const Hit & val, Ray* ray, const Point & point, int height) {
	std::string retval;
	retval += "Pixel: [" + to_string(point.x) + ", " + to_string(point.y) + "] " + 
		"Ray: {" + formatted_to_string(ray->origin.x) + " " + formatted_to_string(ray->origin.y) + " " + formatted_to_string(ray->origin.z) + "} -> {" +
		formatted_to_string(ray->direction.x) + " " + formatted_to_string(ray->direction.y) + " " + formatted_to_string(ray->direction.z) + "}\n";
	if (val.isHit) {
		retval += "T = " + formatted_to_string(val.t) + "\n";
		retval += "Object Type: " + val.objType + "\n";
		retval += "Color: " + formatted_to_string(val.color.x) + " " + formatted_to_string(val.color.y) + " " + formatted_to_string(val.color.z) + "\n";
	}
	else {
		retval += "No Hit\n";
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
	else if (command == "render") {
		bool isAltBRDF = false;
		parseRender(argc, argv, image, &isAltBRDF);
		renderScene(image.width, image.height, scene, isAltBRDF);
	}
	else if (command == "pixelcolor") {
		bool isAltBRDF = false;
		parsePixelcolor(argc, argv, image, point, &isAltBRDF);
		Ray* ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << pixelcolorToString(value, ray, scene, point, image.height, isAltBRDF);
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