#include "Parser.h"

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

	while (!fileStream.eof()) {
		//TODO: convert tok to lower case maybe
		//Read the first token
		fileStream >> tok;
		//Remove commented line
		if (tok.substr(0, 2) == "//") {
			string trashStr;
			getline(fileStream, trashStr);
		}
		else if (tok == "camera") {
			parseCamera(fileStream, scene);
		}
		else if (tok == "light_source") {
			parseLightSource(fileStream, scene);
		}
		else if (tok == "sphere") {
			parseSphere(fileStream, scene);
		}
		else if (tok == "plane") {
			parsePlane(fileStream, scene);
		}
		else {
			cout << "Bad file\n";
			return nullptr;
		}
	}


	return scene;
}

glm::vec3 parseVec3(istringstream & stream) {
	string tok;
	glm::vec3 returnVector;
	stream >> tok;
	//Remove < and ,
	tok = tok.substr(1, tok.length() - 1);
	returnVector.x = stof(tok);
	//Remove ,
	stream >> tok;
	tok = tok.substr(0, tok.length() - 1);
	returnVector.y = stof(tok);
	//Remove > and , sometimes
	stream >> tok;
	int i = 1;
	if (tok.substr(tok.length() - 1, tok.length()) == ",") {
		i = 2;
	}
	tok = tok.substr(0, tok.length() - i);
	returnVector.z = stof(tok);
	return returnVector;

}

glm::vec4 parseVec4(istringstream & stream) {
	string tok;
	glm::vec4 returnVector;
	stream >> tok;
	//Remove < and ,
	tok = tok.substr(1, tok.length() - 1);
	returnVector.x = stof(tok);
	//Remove ,
	stream >> tok;
	tok = tok.substr(0, tok.length() - 1);
	returnVector.y = stof(tok);
	//Remove ,
	stream >> tok;
	tok = tok.substr(0, tok.length() - 1);
	returnVector.z = stof(tok);
	//Remove > and , sometimes
	int i = 1;
	if (tok.substr(tok.length() - 1, tok.length()) == ",") {
		i = 2;
	}
	tok = tok.substr(0, tok.length() - i);
	returnVector.w = stof(tok);
	return returnVector;
}

void parseCamera(istringstream & stream, Scene* scene) {
	string tok;
	glm::vec3 location, up, right, lookat;
	//TODO: add check to make sure all are set
	stream >> tok;
	if (tok != "{") {
		cout << "Bad camera in file\n";
		return;
	}
	do {
		stream >> tok;
		if (tok == "location") {
			location = parseVec3(stream);
		}
		else if (tok == "up") {
			up = parseVec3(stream);
		}
		else if (tok == "right") {
			right = parseVec3(stream);
		}
		else if (tok == "look_at") {
			lookat = parseVec3(stream);
		}
		else if (tok != "}") {
			cout << "unknown token: " << tok << "\n";
		}
	} while (tok != "}");
	scene->setCamera(location, up, right, lookat);
}

void parseLightSource(istringstream & stream, Scene* scene) {
	string line;
	getline(stream, line);
	float x, y, z, r, g, b;
	sscanf(line.c_str(), " {<%f, %f, %f> color rgb <%f, %f, %f>}", &x, &y, &z, &r, &g, &b);
	scene->addLight(glm::vec3(x, y, z), glm::vec3(r, g, b));
}

void parseSphere(istringstream & stream, Scene* scene) {
	string tok;
	stream >> tok;
	if (tok != "{") {
		cout << "Bad Sphere in file\n";
		return;
	}
	glm::vec3 center = parseVec3(stream);
	stream >> tok;
	float radius = stof(tok);

	stream >> tok;
	glm::vec4 color;
	if (tok == "pigment") {
		color = parsePigment(stream);
	}
	else { cout << "bad pigment\n"; return; }
	stream >> tok;
	Material material;
	if (tok == "finish") {
		material = parseFinish(stream);
	}
	else { cout << "bad finish\n"; return; }
	scene->addObject(new Sphere(color, material, center, radius));
}

void parsePlane(istringstream & stream, Scene* scene) {
	string tok;
	stream >> tok;
	if (tok != "{") {
		cout << "Bad Plane in file\n";
		return;
	}
	glm::vec3 normal = parseVec3(stream);
	stream >> tok;
	float offset = stof(tok);

	stream >> tok;
	glm::vec4 color;
	if (tok == "pigment") {
		color = parsePigment(stream);
	}
	else { cout << "bad pigment\n"; return; }
	stream >> tok;
	Material material;
	if (tok == "finish") {
		material = parseFinish(stream);
	}
	else { cout << "bad finish\n"; return; }
	scene->addObject(new Plane(color, material, normal, offset));
}


glm::vec4 parsePigment(std::istringstream & stream) {
	string tok;
	glm::vec4 color;
	stream >> tok;
	if (tok != "{") {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	stream >> tok;
	if (tok != "color") {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	stream >> tok;
	if (tok == "rgb") {
		color = glm::vec4(parseVec3(stream), 1.0f);
	}
	else if (tok == "rgbf") {
		color = parseVec4(stream);
	}
	else {
		cerr << "Bad Pigment in file\n";
		return color;
	}
	getline(stream, tok);
	return color;
}

Material parseFinish(std::istringstream & stream) {
	string tok;
	Material mat;
	stream >> tok;
	if (tok != "{") {
		cerr << "Bad Pigment in file\n";
		return mat;
	}
	stream >> tok;
	if (tok != "ambient") {
		cerr << "Bad Pigment in file\n";
		return mat;
	}
	stream >> tok;
	mat.ambient = stof(tok);
	stream >> tok;
	if (tok != "diffuse") {
		cerr << "Bad Pigment in file\n";
		return mat;
	}
	stream >> tok;
	mat.diffuse = stof(tok);
	//TODO: Add more material info here

	getline(stream, tok);
	return mat;
}
