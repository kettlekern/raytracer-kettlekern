#pragma once
#ifndef __RAY_MANAGER_H_
#define __RAY_MANAGER_H_

#include <glm/glm.hpp>
#include <vector>
#include "Ray.h"
#include "../Scene/Scene.h"

std::vector<Ray*> genRays(int width, int height, glm::vec3 origin, Scene* scene);

#endif