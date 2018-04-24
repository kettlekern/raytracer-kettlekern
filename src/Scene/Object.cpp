#include "Object.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Material.h"
#include "../Ray/Ray.h"
#include "VectorString.h"

using namespace std;

string Object::toString() {
	string retval = " - Type: " + name + "\n";
	retval       += toStringLocal() +
		            " - Color: " + Parser::vec3ToString(color) + "\n" +
		            mat.toString() + "\n";
	return retval;
}

float Object::quadraticRoot(float a, float b, float c) {
	float first, second;
	first = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	second = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	if (first < 0) {
		return second;
	}
	else if (second < 0) {
		return first;
	}
	else if (first < second) {
		return first;
	}
	else {
		return second;
	}
}
