#include "Parser.h"
#include "Scene/Sphere.h"
#include "Scene/Plane.h"
#include "Scene/Triangle.h"
#include "Scene/Cone.h"

using namespace std;

Scene* parseFile(string filename) {
	Scene* scene = new Scene();
	int id = 0;
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
			parseSphere(*tokenizer, scene, id++);
		}
		else if (tok == "plane") {
			parsePlane(*tokenizer, scene, id++);
		}
		else if (tok == "triangle") {
			parseTriangle(*tokenizer, scene, id++);
		}
		else if (tok == "cone") {
			parseCone(*tokenizer, scene, id++);
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
	tok = tokenizer.getToken();
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

void parseSphere(Tokenizer & tokenizer, Scene* scene, int id) {
	string tok;
	glm::vec3 translate, scale, rotate;
	Sphere* object = nullptr;
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
			if (object == nullptr) {
				color = parsePigment(tokenizer);
			}
			else {
				object->setColor(parsePigment(tokenizer));
			}
		}
		else if (tok == "finish") {
			if (object == nullptr) {
				material = parseFinish(tokenizer, color.a);
			}
			else {
				object->setMaterial(parseFinish(tokenizer, color.a));
			}
		}
		//The object must be fully defined before adding transforms
		else if (tok == "translate") {
			if (object == nullptr) {
				object = new Sphere(color, material, id, center, radius);
			}
			translate = parseTranslate(tokenizer);
			object->addTranslate(translate);
		}
		else if (tok == "scale") {
			if (object == nullptr) {
				object = new Sphere(color, material, id, center, radius);
			}
			scale = parseScale(tokenizer);
			object->addScale(scale);
		}
		else if (tok == "rotate") {
			if (object == nullptr) {
				object = new Sphere(color, material, id, center, radius);
			}
			rotate = parseRotate(tokenizer);
			object->addRotate(rotate);
		}
		else if (tok == "fog") {
			if (object == nullptr) {
				object = new Sphere(color, material, id, center, radius);
			}
			auto noise = new OSN::Noise<4>(324);
			Volumetric* fog = new Volumetric(object->getColor(), noise);
			tok = tokenizer.getToken();
			float fogDensity = stof(tok);
			fog->setWeight(fogDensity);
			object->activateFog(fog);
		}
		else { //unknown token
		}
	}
	if (object == nullptr) {
		object = new Sphere(color, material, id, center, radius);
	}
	object->invertModel();
	scene->addObject(object);
}

void parseCone(Tokenizer & tokenizer, Scene* scene, int id) {
	string tok;
	glm::vec3 translate, scale, rotate;
	Cone* object = nullptr;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cout << "Bad Sphere in file\n";
		return;
	}
	glm::vec3 center = parseVec3(tokenizer);
	tok = tokenizer.getToken();
	float radius = stof(tok);
	tok = tokenizer.getToken();
	float height = stof(tok);
	glm::vec4 color;
	Material material;
	while (tok != "}") {
		tok = tokenizer.getToken();
		if (tok == "pigment") {
			color = parsePigment(tokenizer);
		}
		else if (tok == "finish") {
			material = parseFinish(tokenizer, color.a);
		}
		//The object must be fully defined before adding transforms
		else if (tok == "translate") {
			if (object == nullptr) {
				object = new Cone(color, material, id, center, radius, height);
			}
			translate = parseTranslate(tokenizer);
			object->addTranslate(translate);
		}
		else if (tok == "scale") {
			if (object == nullptr) {
				object = new Cone(color, material, id, center, radius, height);
			}
			scale = parseScale(tokenizer);
			object->addScale(scale);
		}
		else if (tok == "rotate") {
			if (object == nullptr) {
				object = new Cone(color, material, id, center, radius, height);
			}
			rotate = parseRotate(tokenizer);
			object->addRotate(rotate);
		}
		else { //unknown token
		}
	}
	if (object == nullptr) {
		object = new Cone(color, material, id, center, radius, height);
	}
	object->invertModel();
	scene->addObject(object);
}

void parsePlane(Tokenizer & tokenizer, Scene* scene, int id) {
	string tok;
	glm::vec3 translate, scale, rotate;
	Plane* object = nullptr;
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
			material = parseFinish(tokenizer, color.a);
		}
		//The object must be fully defined before adding transforms
		else if (tok == "translate") {
			if (object == nullptr) {
				object = new Plane(color, material, id,  normal, offset);
			}
			translate = parseTranslate(tokenizer);
			object->addTranslate(translate);
		}
		else if (tok == "scale") {
			if (object == nullptr) {
				object = new Plane(color, material, id, normal, offset);
			}
			scale = parseScale(tokenizer);
			object->addScale(scale);
		}
		else if (tok == "rotate") {
			if (object == nullptr) {
				object = new Plane(color, material, id, normal, offset);
			}
			rotate = parseRotate(tokenizer);
			object->addRotate(rotate);
		}
		else { //unknown token
		}
	}
	if (object == nullptr) {
		object = new Plane(color, material, id, normal, offset);
	}
	object->invertModel();
	scene->addObject(object);
}

void parseTriangle(Tokenizer & tokenizer, Scene* scene, int id) {
	string tok;
	tok = tokenizer.getToken();
	Triangle* object = nullptr;
	if (tok != "{") {
		cout << "Bad Triangle in file\n";
		return;
	}
	glm::vec3 pointA = parseVec3(tokenizer);
	glm::vec3 pointB = parseVec3(tokenizer);
	glm::vec3 pointC = parseVec3(tokenizer);
	glm::vec3 translate, scale, rotate;
	glm::vec4 color;
	Material material;
	while (tok != "}") {
		tok = tokenizer.getToken();
		if (tok == "pigment") {
			color = parsePigment(tokenizer);
		}
		else if (tok == "finish") {
			material = parseFinish(tokenizer, color.a);
		}
		//The object must be fully defined before adding transforms
		else if (tok == "translate") {
			if (object == nullptr) {
				object = new Triangle(color, material, id, pointA, pointB, pointC);
			}
			translate = parseTranslate(tokenizer);
			object->addTranslate(translate);
		}
		else if (tok == "scale") {
			if (object == nullptr) {
				object = new Triangle(color, material, id, pointA, pointB, pointC);
			}
			scale = parseScale(tokenizer);
			object->addScale(scale);
		}
		else if (tok == "rotate") {
			if (object == nullptr) {
				object = new Triangle(color, material, id, pointA, pointB, pointC);
			}
			rotate = parseRotate(tokenizer);
			object->addRotate(rotate);
		}
		else { //unknown token
		}
	}
	if (object == nullptr) {
		object = new Triangle(color, material, id, pointA, pointB, pointC);
	}
	object->invertModel();
	scene->addObject(object);
}


glm::vec4 parsePigment(Tokenizer & tokenizer) {
	glm::vec4 color;
	string tok;
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
		color = glm::vec4(parseVec3(tokenizer), 0.0f);
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
	return parseFinish(tokenizer, 0.0f);
}

glm::vec3 parseTranslate(Tokenizer & tokenizer)
{
	return parseVec3(tokenizer);
}

glm::vec3 parseScale(Tokenizer & tokenizer)
{
	return parseVec3(tokenizer);
}

glm::vec3 parseRotate(Tokenizer & tokenizer)
{
	return parseVec3(tokenizer);
}

Material parseFinish(Tokenizer & tokenizer, float refraction) {
	Material mat;
	string tok;
	mat.refraction = refraction;
	tok = tokenizer.getToken();
	if (tok != "{") {
		cerr << "Bad Finish in file\n";
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
		else if (tok == "reflection") {
			tok = tokenizer.getToken();
			mat.reflection = stof(tok);
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
