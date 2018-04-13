#include "Material.h"
#include <string>
using namespace std;


string Material::toString() {
	string retval = " - Material:\n";
	retval       += "    - Ambient: " + to_string(ambient) + "\n" + 
		            "    - Diffuse: " + to_string(diffuse);
	return retval;
}