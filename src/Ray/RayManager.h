#pragma once
#ifndef __RAY_MANAGER_H_473_
#define __RAY_MANAGER_H_473_

#include <glm/glm.hpp>
#include <vector>
#include "../Hit.h"
#include "Ray.h"
#include "../Scene/Scene.h"

std::vector<Ray*> genRays(int width, int height, glm::vec3 origin, Scene* scene);
Hit collide(Scene* scene, Ray* ray);

#endif