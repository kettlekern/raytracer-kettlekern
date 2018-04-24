#pragma once
#ifndef __FRAGMENT_473_
#define __FRAGMENT_473_
#include <glm/glm.hpp>
#include <vector>
#include "Material.h"
#include "Object.h"
#include "Scene.h"
#include "../Hit.h"

class Fragment {
	glm::vec3 position;
	glm::vec3 color;
	Material mat;
	float t;
	Object* obj;
	Camera* cam;

public:
	Fragment(Hit hit) {}
	//Color the fragment using the Cook-Torrance lighting approximation 
	glm::vec3 CookTorrance(const std::vector<Light *> & lights);
	//Color the fragment using the blinn-phong lighting approximation 
	glm::vec3 BlinnPhong(const std::vector<Light *> & lights);
	glm::vec3 Fragment::BlinnPhongObject(glm::vec3 position, glm::vec3 normal, glm::vec3 ambient, glm::vec3 diffuseColor, glm::vec3 specularColor, float shine, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor);
	//Color the fragment using the given lighting algorithm
	void computeLighting(glm::vec3 (*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights);
	glm::vec3 getColor() { return color; }
	glm::vec3 getPosition() { return position; }
};



#endif