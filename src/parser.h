#pragma once
#ifndef __POV_FILE_PARSER_H_
#define __POV_FILE_PARSER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include "Scene\Scene.h"


Scene* parseFile(std::string filename);
void parseCamera(std::istringstream & fileStream, Scene* scene);
void parseLightSource(std::istringstream & fileStream, Scene* scene);
void parseSphere(std::istringstream & fileStream, Scene* scene);
void parsePlane(std::istringstream & fileStream, Scene* scene);
glm::vec4 parsePigment(std::istringstream & fileStream);
Material parseFinish(std::istringstream & fileStream);






#endif
