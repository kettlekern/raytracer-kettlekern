#pragma once
#ifndef __FRAGMENT_473_
#define __FRAGMENT_473_
#include <glm/glm.hpp>
#include <vector>
#include "../Scene/Material.h"
#include "../Scene/Object.h"
#include "../Scene/Scene.h"
#include "../Hit.h"
#include "../Ray/Ray.h"

//May want to hide this in a namespace
enum LIGHTMODE {
	BLINN_PHONG,
	COOK_TORRANCE,
	FLAT
};

class Fragment {
	glm::vec3 position;
	glm::vec3 fragColor;
	Material mat;
	float t;
	Object* obj;
	Ray* ray;

	glm::vec3 clampColor(glm::vec3 color);
	bool inShadow(Light* light, Scene* scene);
	//Color the fragment using the Cook-Torrance lighting approximation 
	glm::vec3 CookTorrance(Scene* scene);
	glm::vec3 CookTorranceDiffuse(float Kd, glm::vec3 normal, glm::vec3 lightDir, glm::vec3 diffuseColor, glm::vec3 lightColor);
	glm::vec3 CookTorranceSpecular(float Ks, glm::vec3 normal, glm::vec3 lightDir, glm::vec3 viewDir, glm::vec3 specularColor, glm::vec3 lightColor, glm::vec3 H, float ior, float roughness);

	float CookTorranceFresnel(float ior, const glm::vec3 &viewDir, const glm::vec3 &H);
	float CookTorranceD(float alphasq, const glm::vec3 & normal, const glm::vec3 & H);
	float CookTorranceG(float alphasq, const glm::vec3 & normal, const glm::vec3 & H, const glm::vec3 & VorL);

	glm::vec3 CookTorranceObject(glm::vec3 position, glm::vec3 normal, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float roughness, float ior, float specular, float diffuse);
	
	//Color the fragment using the blinn-phong lighting approximation 
	glm::vec3 BlinnPhong(Scene* scene);
	glm::vec3 BlinnPhongObject(glm::vec3 position, glm::vec3 normal, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float shine, float diffuse, float specular);
	
	glm::vec3 calcAmbientLight();

	//TODO: Extract these functions from this class maybe
	//returns the dot product of the first and second clamped between 0 and 1
	float cdot(glm::vec3 first, glm::vec3 second);
	//returns 1 if val > 0, 0 otherwise
	int chiPos(float val);

	void colorFrag(Scene* scene, LIGHTMODE lightingType, int maxBounces);
	void colorFrag(Scene* scene, LIGHTMODE lightingType, int maxBounces, bool verbose);
	glm::vec3 calcLocalColor(Scene * scene, LIGHTMODE lightMode);
	glm::vec3 calcReflectionColor(Scene * scene, LIGHTMODE lightMode, int maxBounces, bool verbose);
	glm::vec3 calcRefractionColor(Scene * scene, LIGHTMODE lightMode, int maxBounces, bool verbose);
	//The second vector needs more support to function for all cases, for now we will just assume air + object collisions
	glm::vec3 calcRefractionVector(glm::vec3 direction, glm::vec3 normal, float ior1, float ior2);

public:
	Fragment(const Hit & hit, Scene* scene, Ray* ray);

	//This should only be called externally, and only once. Use the bounce counting version for all other calls.
	void colorFrag(Scene* scene, LIGHTMODE lightingType);
	void colorFrag(Scene* scene, LIGHTMODE lightingType, bool verbose);


	bool isHit() { return obj != nullptr; }
	glm::vec3 getColor() { return fragColor; }
	glm::vec3 getPosition() { return position; }
	std::string toString();
};



#endif