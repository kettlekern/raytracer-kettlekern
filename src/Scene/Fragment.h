#pragma once
#ifndef __FRAGMENT_473_
#define __FRAGMENT_473_
#include <glm/glm.hpp>
#include <vector>
#include "Material.h"
#include "Object.h"
#include "Scene.h"
#include "../Hit.h"

//May want to hide this in a namespace
enum LIGHTMODE {
	BLINN_PHONG,
	COOK_TORRANCE,
	FLAT
};

class Fragment {
	glm::vec3 position;
	glm::vec3 color;
	Material mat;
	float t;
	Object* obj;
	Camera* cam;
	//Color the fragment using the Cook-Torrance lighting approximation 
	glm::vec3 CookTorrance(const std::vector<Light *> & lights);
	//Color the fragment using the blinn-phong lighting approximation 
	glm::vec3 BlinnPhong(const std::vector<Light *> & lights);
	glm::vec3 BlinnPhongObject(glm::vec3 position, glm::vec3 normal, glm::vec3 ambient, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float shine);
	//Color the fragment using the given lighting algorithm
	void computeLighting(glm::vec3(*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights);


public:
	Fragment(const Hit & hit, Scene* scene);
	void colorFrag(Scene* scene, LIGHTMODE lightingType);
	bool isHit() { return obj != nullptr; }
	glm::vec3 getColor() { return color; }
	glm::vec3 getPosition() { return position; }
};



#endif