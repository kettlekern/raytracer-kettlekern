#include "Parser.h"
#include "Scene/Sphere.h"
#include "Scene/Plane.h"
#include "Scene/Triangle.h"

using namespace std;

Scene* parseFile(string filename) {
	Scene* scene = new Scene();
	//Open file
	ifstream file(filename);
	stringstream fileString;
	string tok;
	if (file.is_open()) {
		//dump file into a string
		fileString << file.rdbuf();
	}
	else { 
		cout << "Could not open file"; 
	}
	file.close();
	//Turn string into string stream
	istringstream fileStream(fileString.str());
	Tokenizer* tokenizer = new Tokenizer(&fileStream);
	//Read the first token
	tok = tokenizer->getToken();

	while (!fileStream.eof()) {
		//TODO: convert tok to lower case maybe
		//Remove commented line
		if (tok.substr(0, 2) == "//") {
			string trashStr;
			tokenizer->getLine(tok);
		}
		else if (tok == "camera") {
			parseCamera(*tokenizer, scene);
		}
		else if (tok == "light_source") {
			parseLightSource(*tokenizer, scene);
		}
		else if (tok == "sphere") {
			parseSphere(*tokenizer, scene);
		}
		else if (tok == "plane") {
			parsePlane(*tokenizer, scene);
		}
		else if (tok == "triangle") {
			parseTriangle(*tokenizer, scene);
		}
		else {
			cout << "Bad file\n" << tok << "\n";
			return scene;
		}
		//Read the next token
		tok = tokenizer->getToken();
	}


	return scene;
}

glm::vec3 parseVec3(Tokenizer & tokenizer) {
	string tok;
	glm::vec3 returnVector;
	tok = tokenizer.getToken();
	returnVector.x = stof(tok);
	tok = tokenizer.getToken();
	returnVector.y = stof(tok);
	tok = tokenizer.getToken();
	returnVector.z = stof(tok);
	return returnVector;

}

glm::vec4 parseVec4(Tokenizer & tokenizer) {
	string tok;
	glm::vec4 returnVector;
	tok = tokenizer.getToken();
	returnVector.x = stof(tok);
	tok = tokenizer.getToken();
	returnVector.y = stof(tok);
	tok = tokenizer.getToken();
	returnVector.z = stof(tok);
	returnVector.w = stof(tok);
	return returnVector;
}

void parseCamera(Tokenizer & tokenizer, Scene* scene) {
	string tok;
	glm::vec3 location, up, right, lookat;
	//TODO: add check to make sure all are set
	tok = tokenizer.getToken();
	if (tok != "{") {
		cout << "Bad camera in file\n";
		return;
	}
	do {
		tok = tokenizer.getToken();
		if (tok == "location") {
			location = parseVec3(tokenizer);
		}
		else if (tok == "up") {
			up = parseVec3(tokenizer);
		}
		else if (tok == "right") {
			right = parseVec3(tokenizer);
		}
		else if (tok == "look_at") {
			lookat = parseVec3(tokenizer);
		}
		else if (tok != "}") {
			cout << "unknown token: " << tok << "\n";
		}
	} while (tok != "}");
	scene->setCamera(location, up, right, lookat);
}

void parseLightSource(Tokenizer & tokenizer, Scene* scene) {
	string line;
	tokenizer.getLine(line);
	float x, y, z, r, g, b;
	sscanf(line.c_str(), " {<%f, %f, %f> color rgb <%f, %f, %f>}", &x, &y, &z, &r, &g, &b);
	scene->addLight(glm::vec3(x, y, z), glm::vec3(r, g, b));
}

void parseSphere(Tokenizer & tokenizer, Scene* scene) {
	string tok;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cout << "Bad Sphere in file\n";
		return;
	}
	glm::vec3 center = parseVec3(tokenizer);
	tok = tokenizer.getToken();
	float radius = stof(tok);
	glm::vec4 color;
	Material material;
	while (tok != "}") {
		tok = tokenizer.getToken();
		if (tok == "pigment") {
			color = parsePigment(tokenizer);
		}
		else if (tok == "finish") {
			material = parseFinish(tokenizer);
		}
		else { //unknown token
		}
	}
	scene->addObject(new Sphere(color, material, center, radius));
}

void parsePlane(Tokenizer & tokenizer, Scene* scene) {
	string tok;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cout << "Bad Plane in file\n";
		return;
	}
	glm::vec3 normal = parseVec3(tokenizer);
	tok = tokenizer.getToken();
	float offset = stof(tok);
	glm::vec4 color;
	Material material;
	while (tok != "}") {
		tok = tokenizer.getToken();
		if (tok == "pigment") {
			color = parsePigment(tokenizer);
		}
		else if (tok == "finish") {
			material = parseFinish(tokenizer);
		}
		else { //unknown token
		}
	}
	scene->addObject(new Plane(color, material, normal, offset));
}

void parseTriangle(Tokenizer & tokenizer, Scene* scene) {
	string tok;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cout << "Bad Triangle in file\n";
		return;
	}
	glm::vec3 pointA = parseVec3(tokenizer);
	glm::vec3 pointB = parseVec3(tokenizer);
	glm::vec3 pointC = parseVec3(tokenizer);
	glm::vec4 color;
	Material material;
	while (tok != "}") {
		tok = tokenizer.getToken();
		if (tok == "pigment") {
			color = parsePigment(tokenizer);
		}
		else if (tok == "finish") {
			material = parseFinish(tokenizer);
		}
		else { //unknown token
		}
	}
	scene->addObject(new Triangle(color, material, pointA, pointB, pointC));
}


glm::vec4 parsePigment(Tokenizer & tokenizer) {
	string tok;
	glm::vec4 color;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	tok = tokenizer.getToken();
	if (tok != "color") {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	tok = tokenizer.getToken();
	if (tok == "rgb") {
		color = glm::vec4(parseVec3(tokenizer), 1.0f);
	}
	else if (tok == "rgbf") {
		color = parseVec4(tokenizer);
	}
	else {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	tokenizer.getToken();
	return color;
}

Material parseFinish(Tokenizer & tokenizer) {
	string tok;
	Material mat;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cerr << "Bad Pigment in file\n";
		return mat;
	}
	tok = tokenizer.getToken();
	while (tok != "}") {
		if (tok == "ambient") {
			tok = tokenizer.getToken();
			mat.ambient = stof(tok);
		}
		else if (tok == "diffuse") {
			tok = tokenizer.getToken();
			mat.diffuse = stof(tok);
		}
		else if (tok == "specular") {
			tok = tokenizer.getToken();
			mat.specular = stof(tok);
		}
		else if (tok == "roughness") {
			tok = tokenizer.getToken();
			mat.roughness = stof(tok);
		}
		else if (tok == "metallic") {
			tok = tokenizer.getToken();
			mat.metallic = stof(tok);
		}
		else if (tok == "ior") {
			tok = tokenizer.getToken();
			mat.ior = stof(tok);
		}
		//TODO: Add more material info here
		tok = tokenizer.getToken();
	}
	return mat;
}
