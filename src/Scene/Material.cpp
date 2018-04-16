#include "Material.h"
#include <string>
using namespace std;


string Material::toString() {
	string retval = " - Material:\n";
	retval       += "    - Ambient: " + formatted_to_string(ambient) + "\n" + 
		            "    - Diffuse: " + formatted_to_string(diffuse);
	return retval;
}