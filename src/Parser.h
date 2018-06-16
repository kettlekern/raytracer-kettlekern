#pragma once
#ifndef __POV_FILE_PARSER_H_473_
#define __POV_FILE_PARSER_H_473_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Scene/Scene.h"
#include "Tokenizer.h"
#include "FormattedToString.h"
#include "Flags.h"


Scene* parseFile(std::string filename, const Flags & flags);
void parseCamera(Tokenizer & fileStream, Scene* scene);
void parseLightSource(Tokenizer & fileStream, Scene* scene);
void parseSphere(Tokenizer & fileStream, Scene* scene, int id, const Flags & flags);
void parseBox(Tokenizer & fileStream, Scene* scene, int id, const Flags & flags);
void parseCone(Tokenizer & fileStream, Scene* scene, int id);
void parseTriangle(Tokenizer & fileStream, Scene* scene, int id);
void parsePlane(Tokenizer & fileStream, Scene* scene, int id);
glm::vec4 parsePigment(Tokenizer & fileStream);
Material parseFinish(Tokenizer & fileStream, float refraction);
Material parseFinish(Tokenizer & fileStream);
glm::vec3 parseTranslate(Tokenizer & fileStream);
glm::vec3 parseScale(Tokenizer & fileStream);
glm::vec3 parseRotate(Tokenizer & fileStream);






#endif
