#pragma once
#ifndef __POV_FILE_PARSER_H_473_
#define __POV_FILE_PARSER_H_473_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Scene\Scene.h"


Scene* parseFile(std::string filename);
void parseCamera(std::istringstream & fileStream, Scene* scene);
void parseLightSource(std::istringstream & fileStream, Scene* scene);
void parseSphere(std::istringstream & fileStream, Scene* scene);
void parsePlane(std::istringstream & fileStream, Scene* scene);
glm::vec4 parsePigment(std::istringstream & fileStream);
Material parseFinish(std::istringstream & fileStream);






#endif
