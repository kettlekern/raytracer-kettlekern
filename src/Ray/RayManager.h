#pragma once
#ifndef __RAY_MANAGER_H_473_
#define __RAY_MANAGER_H_473_

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "../Hit.h"
#include "Ray.h"
#include "../Scene/Scene.h"

std::vector<Ray> genRays(int width, int height, Scene* scene);
Ray genRay(int width, int height, Scene* scene, int i, int j);
Hit collide(Scene* scene, Ray ray);
void castRays(int width, int height, Scene* scene);
Hit setHit(float t, Object* obj, Ray ray);
void renderScene(int width, int height, Scene* scene, bool useCookTorrance);

#endif