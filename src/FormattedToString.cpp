#include "FormattedToString.h"

#define EPS .00005f

std::string formatted_to_string(float f){
	std::string str = std::to_string(f + EPS);
	return str.substr(0, str.length() - 2);
}