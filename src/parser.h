#pragma once
#ifndef __POV_FILE_PARSER_H_473_
#define __POV_FILE_PARSER_H_473_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Scene\Scene.h"
#include "Tokenizer.h"


Scene* parseFile(std::string filename);
void parseCamera(Tokenizer & fileStream, Scene* scene);
void parseLightSource(Tokenizer & fileStream, Scene* scene);
void parseSphere(Tokenizer & fileStream, Scene* scene);
void parsePlane(Tokenizer & fileStream, Scene* scene);
glm::vec4 parsePigment(Tokenizer & fileStream);
Material parseFinish(Tokenizer & fileStream);






#endif
