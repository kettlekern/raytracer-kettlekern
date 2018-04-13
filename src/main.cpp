
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Parser.h"


using namespace std;




int runCommand(int argc, char** argv) {
	string command = argv[1];
	string filename = argv[2];
	Scene* scene = parseFile(filename);
	if (command == "raycast") {
		runRaycast(argc, argv);
	}
	else if (command == "sceneinfo") {
		scene->printScene();
	}
	else if (command == "pixelray") {
		runPixelray(argc, argv);
	}
	else if (command == "firsthit") {
		runFirsthit(argc, argv);
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