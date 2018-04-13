#include "Object.h"
#include "VectorString.h"

using namespace std;

string Object::toString() {
	string retval = " - Type: " + name + "\n";
	retval       += toStringLocal() +
		            " - Color: " + vec3ToString(color) + "\n" +
		            mat.toString();
	return retval;
}
