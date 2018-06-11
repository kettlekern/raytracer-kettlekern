
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"
#include "Ray/RayManager.h"
#include "Renderer/Fragment.h"
#include "main.h"
#include "Flags.h"

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

void parseFlags(int argc, char ** argv, Flags & flags)
{
	string altBRDF = "-altbrdf";
	string Beers = "-beers";
	string Fresnel = "-fresnel";
	string superSample = "-ss=";
	string SDS = "-sds";
	string temp;
	int argcOffset = 5;
	argc -= argcOffset;
	while (argc > 0) {
		temp = argv[argcOffset];
		if (altBRDF.compare(temp) == 0) {
			flags.isAltBRDF = true;
		}
		else if (Beers.compare(temp) == 0) {
			flags.useBeers = true;
		}
		else if (Fresnel.compare(temp) == 0) {
			flags.useFresnel = true;
		}
		else if (SDS.compare(temp) == 0) {
			flags.useSDS = true;
		}
		else if (superSample.compare(temp.substr(0,4)) == 0) {
			flags.superSampleCount = stoi(temp.substr(4, temp.length()));
		}
		argc--;
		argcOffset++;
	}
}

void parseRender(int argc, char** argv, ImageCoords & image, Flags & flags) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	parseFlags(argc, argv, flags);
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

std::string pixelrayToString(const Hit & val, Ray ray, const Point & rayLoc, int height) {
	std::string retval;
	retval += "Pixel: [" + to_string(rayLoc.x) + ", " + to_string(rayLoc.y) + "] " +
			"Ray: {" + formatted_to_string(ray.origin.x) + " " + formatted_to_string(ray.origin.y) + " " + formatted_to_string(ray.origin.z) + "} -> {" +
			formatted_to_string(ray.direction.x) + " " + formatted_to_string(ray.direction.y) + " " + formatted_to_string(ray.direction.z) + "}\n";
	return retval;
}

std::string pixelcolorToString(const Hit & val, Ray ray, Scene* scene, const Point & rayLoc, int height, bool isAltBRDF) {
	std::string retval;
	std::string BRDFType = isAltBRDF ? "Cook-Torrance" : "Blinn-Phong";
	Fragment frag(val, scene, ray);
	frag.colorFrag(scene, isAltBRDF ? COOK_TORRANCE : BLINN_PHONG);
	retval += pixelrayToString(val, ray, rayLoc, height);
	retval += "T = " + formatted_to_string(val.t) + "\n";
	retval += "Object Type: " + val.objType + "\n";
	retval += "BRDF: " + BRDFType + "\n";
	//TODO: update this and pass in a fragment instead
	retval += "Color: (" + to_string((int)(frag.getColor().r * 255 + 0.5f)) + ", " + to_string((int)(frag.getColor().g * 255 + 0.5f)) + ", " + to_string((int)(frag.getColor().b * 255 + 0.5f)) + ")\n";
	return retval;
}

std::string firstHitToString(const Hit & val, Ray ray, const Point & point, int height) {
	std::string retval;
	retval += "Pixel: [" + to_string(point.x) + ", " + to_string(point.y) + "] " + 
		"Ray: {" + formatted_to_string(ray.origin.x) + " " + formatted_to_string(ray.origin.y) + " " + formatted_to_string(ray.origin.z) + "} -> {" +
		formatted_to_string(ray.direction.x) + " " + formatted_to_string(ray.direction.y) + " " + formatted_to_string(ray.direction.z) + "}\n";
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
	Flags flags;
	if (command == "raycast") {
		parseRaycast(argc, argv, image);
		castRays(image.width, image.height, scene);
	}
	else if (command == "render") {
		parseRender(argc, argv, image, flags);
		renderScene(image.width, image.height, scene, flags);
	}
	else if (command == "pixelcolor") {
		bool isAltBRDF = false;
		parsePixelcolor(argc, argv, image, point, &isAltBRDF);
		Ray ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << pixelcolorToString(value, ray, scene, point, image.height, isAltBRDF);
	}
	else if (command == "sceneinfo") {
		scene->printScene();
	}
	else if (command == "pixelray") {
		parsePixelray(argc, argv, image, point);
		Ray ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		cout << pixelrayToString(value, ray, point, image.height);
	}
	else if (command == "printrays") {
		parsePixelray(argc, argv, image, point);
		Ray ray = genRay(image.width, image.height, scene, point.x, point.y);
		Hit value = collide(scene, ray);
		Fragment frag(value, scene, ray);
		cout << pixelcolorToString(value, ray, scene, point, image.height, false);
		//Verbose mode doesn't really do anything helpful right now
		frag.colorFrag(scene, BLINN_PHONG, false);
	}
	else if (command == "firsthit") {
		parseFirstHit(argc, argv, image, point);
		Ray ray = genRay(image.width, image.height, scene, point.x, point.y);
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