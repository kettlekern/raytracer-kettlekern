#include "FormattedToString.h"

#define EPS .00005f

std::string formatted_to_string(float f){
	float eps = EPS;
	if (f < 0) {
		eps = -eps;
	}
	std::string str = std::to_string(f + eps);
	return str.substr(0, str.length() - 2);
}