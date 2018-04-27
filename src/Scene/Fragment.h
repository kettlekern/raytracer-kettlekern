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
	Camera cam;
	glm::vec3 clampColor(glm::vec3 color);
	bool inShadow(Light* light, Scene* scene);
	//Color the fragment using the Cook-Torrance lighting approximation 
	glm::vec3 CookTorrance(Scene* scene);
	glm::vec3 CookTorranceDiffuse(float Kd, glm::vec3 normal, glm::vec3 lightDir, glm::vec3 diffuseColor, glm::vec3 lightColor);
	glm::vec3 CookTorranceSpecular(float Ks, glm::vec3 normal, glm::vec3 lightDir, glm::vec3 viewDir, glm::vec3 specularColor, glm::vec3 lightColor, glm::vec3 H, float ior, float roughness);

	void CookTorranceFresnel(float ior, float &F, const glm::vec3 &viewDir, const glm::vec3 &H);
	float CookTorranceD(float alphasq, const glm::vec3 & normal, const glm::vec3 & H);
	float CookTorranceG(float alphasq, const glm::vec3 & normal, const glm::vec3 & H, const glm::vec3 & VorL);

	glm::vec3 CookTorranceObject(glm::vec3 position, glm::vec3 normal, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float roughness, float ior, float specular, float diffuse);
	//Color the fragment using the blinn-phong lighting approximation 
	glm::vec3 BlinnPhong(Scene* scene);
	glm::vec3 BlinnPhongObject(glm::vec3 position, glm::vec3 normal, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float shine, float diffuse, float specular);
	//Color the fragment using the given lighting algorithm
	void computeLighting(glm::vec3(*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights);
	
	//TODO: Extract these functions from this class maybe
	//returns the dot product of the first and second clamped between 0 and 1
	float cdot(glm::vec3 first, glm::vec3 second);
	//returns 1 if val > 0, 0 otherwise
	int chiPos(float val);

public:
	Fragment(const Hit & hit, Scene* scene);
	void colorFrag(Scene* scene, LIGHTMODE lightingType);
	bool isHit() { return obj != nullptr; }
	glm::vec3 getColor() { return color; }
	glm::vec3 getPosition() { return position; }
};



#endif