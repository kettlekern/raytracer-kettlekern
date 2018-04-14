
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"


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
void parsePixelray(int argc, char** argv, Image & image, Point point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	point.y = stoi(argv[6]);
}
void parseFirstHit(int argc, char** argv, Image & image, Point point) {
	image.width = stoi(argv[3]);
	image.height = stoi(argv[4]);
	point.x = stoi(argv[5]);
	point.y = stoi(argv[6]);
}

int runCommand(int argc, char** argv) {
	string command = argv[1];
	string filename = argv[2];
	Scene* scene = parseFile(filename);
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
	}
	else if (command == "firsthit") {
		parseFirstHit(argc, argv, image, point);
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