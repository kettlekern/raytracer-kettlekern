#include "FormattedToString.h"

std::string formatted_to_string(float f){
	std::string str = std::to_string(f);
	return str.substr(0, str.length() - 2);
}