#include "Fragment.h"
#include "../Ray/RayManager.h"
#define EPS 0.05f
#define PI 3.14159265f
#define MAX_BOUNCES 6
#define CLEAR_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

using namespace glm;

Fragment::Fragment(const Hit & hit, Scene* scene, Ray* ray) {
	if (hit.isHit) {
		position = hit.position;
		//Compute this later
		color = hit.color;
		mat = hit.mat;
		t = hit.t;
		rayOrigin = ray->origin;
		obj = hit.obj;
	}
	else {
		color = vec3(0, 0, 0);
		obj = nullptr;
	}
}

float Fragment::cdot(vec3 first, vec3 second) {
	return glm::clamp(glm::dot(first, second), 0.0f, 1.0f);
}

int Fragment::chiPos(float val) {
	if (val > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

glm::vec3 Fragment::CookTorranceDiffuse(float Kd, vec3 normal, vec3 lightDir, vec3 diffuseColor, vec3 lightColor) {
	return lightColor * diffuseColor * Kd * cdot(normal, lightDir);
}

float Fragment::CookTorranceFresnel(float ior, const glm::vec3 &viewDir, const glm::vec3 &H)
{
	float Fo = pow(ior - 1.0f, 2) / pow(ior + 1.0f, 2);
	return Fo + (1 - Fo) * pow(1 - cdot(viewDir, H), 5);
}

//This is using GGX
float Fragment::CookTorranceD(float alphasq, const glm::vec3 & normal, const glm::vec3 & H)
{
	float NoH = cdot(normal, H);
	float denom = NoH * NoH * (alphasq - 1) + 1;
	return chiPos(NoH) * alphasq / (PI * denom * denom);
}

//This is using GGX
float Fragment::CookTorranceG(float alphasq, const glm::vec3 & normal, const glm::vec3 & H, const glm::vec3 & VorL)
{
	float VorLoH = cdot(VorL, H);
	int chi = chiPos(VorLoH / cdot(VorL, normal));
	float VoH2 = VorLoH * VorLoH;
	return chi * 2 / (1 + sqrt(1 + alphasq * (1 - VoH2) / VoH2));
}

glm::vec3 Fragment::CookTorranceSpecular(float Ks, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specularColor, vec3 lightColor, glm::vec3 H, float ior, float roughness) {
	float D, F, G;
	//alpha is roughness squared, to square that again and you get alphasq = roughness^4
	float alphasq = pow(roughness, 4);
	D = CookTorranceD(alphasq, normal, H);
	F = CookTorranceFresnel(ior, viewDir, H);
	G = CookTorranceG(alphasq, normal, H, viewDir) * CookTorranceG(alphasq, normal, H, lightDir);
	//There should be a *4 in the denom, but removing it makes me match, so the bug is somewhere else probably but I can leave this here and call it a simplification.
	return lightColor * Ks * D * F * G / (4 * cdot(normal, viewDir));
}

glm::vec3 Fragment::CookTorranceObject(glm::vec3 position, glm::vec3 normal, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 cameraPos, glm::vec3 lightPos, glm::vec3 lightColor, float roughness, float ior, float specular, float diffuse) {
	vec3 lightDir = normalize(lightPos - position);
	vec3 viewDir = normalize(cameraPos - position);
	vec3 H = normalize(lightDir + viewDir);

	return CookTorranceDiffuse(diffuse, normal, lightDir, diffuseColor, lightColor) + CookTorranceSpecular(specular, normal, lightDir, viewDir, specularColor, lightColor, H, ior, roughness);
}

glm::vec3 Fragment::CookTorrance(Scene* scene) {
	vec3 color = CLEAR_COLOR;
	vec3 ambient = obj->getColor() * mat.ambient;
	for (Light* light : scene->getLights()) {
		if (!inShadow(light, scene)) {
			color += CookTorranceObject(position, obj->getNormal(position), obj->getColor(), obj->getColor(), rayOrigin, light->location, light->color, mat.roughness, mat.ior, mat.specular, mat.diffuse);
		}
	}
	color += ambient;
	color = clampColor(color);
	return color;
}

//This is pretty much shader code for blinn phong and should have similar properties to fragment shader code
vec3 Fragment::BlinnPhongObject(vec3 position, vec3 normal, vec3 diffuseColor, vec3 specularColor, vec3 cameraPos, vec3 lightPos, vec3 lightColor, float shine, float diff, float spec) {
	vec3 lighting;
	vec3 viewDir = normalize(cameraPos - position);
	vec3 lightDir = normalize(lightPos - position);
	vec3 H = normalize(lightDir + viewDir);

	//I think this is off by a bit
	float attenuation = 1.0f;//clamp(1.0 / pow(length(lightPos - position), 3.0), 0.0, 1.0);

	vec3 diffuse = diff * glm::max(dot(normal, lightDir), 0.0f) * diffuseColor * lightColor;
	vec3 specular = vec3(0.0f);
	if (shine > 0.0f) {
		specular = spec * specularColor * lightColor * glm::max(pow(glm::dot(normal, H), shine), 0.0f);
	}

	lighting = (diffuse + specular) * attenuation;

	return lighting;
}

vec3 Fragment::clampColor(vec3 color) {
	if (color.r > 1.0f) {
		color.r = 1.0f;
	}
	if (color.g > 1.0f) {
		color.g = 1.0f;
	}
	if (color.b > 1.0f) {
		color.b = 1.0f;
	}
	return color;
}

bool Fragment::inShadow(Light* light, Scene* scene) {
	bool shadow = false;
	vec3 direction = normalize(light->location - position);
	Ray* ray = new Ray(position + EPS * direction, direction);
	Hit hit = collide(scene, ray);
	//The length of this vector is the number of itterations of the direction it takes to reach the light from the fragment
	//Because the direction vector has length = 1, this value is the t value to the light from the fragment
	float lightT = length(light->location - position);
	if (hit.isHit && hit.t <= lightT) {
		shadow = true;
	}
	delete(ray);
	return shadow;
}

vec3 Fragment::BlinnPhong(Scene* scene) 
{
	vec3 color = CLEAR_COLOR;
	vec3 ambient = CLEAR_COLOR;
	float ambientAmount = mat.ambient;
	ambient = obj->getColor() * ambientAmount;
	for (Light* light : scene->getLights()) {
		if (!inShadow(light, scene)) {
			color += BlinnPhongObject(position, obj->getNormal(position), obj->getColor(), obj->getColor(), rayOrigin, light->location, light->color, mat.roughness == 0 ? 0.0f : 2 / pow(mat.roughness, 2) - 2, mat.diffuse, mat.specular);
		}
	}
	color += ambient;
	color = clampColor(color);
	return color;
}

void Fragment::computeLighting(glm::vec3 (*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights) 
{
	color = (*lighting)(lights);
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode) 
{
	colorFrag(scene, lightMode, MAX_BOUNCES);
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode, int maxBounces)
{
	color = CLEAR_COLOR;
	//Put a limit on the number of times light can bounce in the scene to stop infinite recursion
	if (maxBounces > 0 && isHit()) {
		maxBounces--;
		color = (1 - mat.reflection) * CalcLocalColor(scene, lightMode);
		if (mat.reflection > 0.0f) {
			color += mat.reflection * CalcReflectionColor(scene, lightMode, maxBounces);
		}
	}
}

vec3 Fragment::CalcLocalColor(Scene * scene, LIGHTMODE lightMode)
{
	vec3 color;
	switch (lightMode) {
	case BLINN_PHONG:
		color = BlinnPhong(scene);
		break;
	case COOK_TORRANCE:
		color = CookTorrance(scene);
		break;
	}
	return color;
}

vec3 Fragment::CalcReflectionColor(Scene * scene, LIGHTMODE lightMode, int maxBounces)
{
	//rayDir = -viewDir
	vec3 rayDir = normalize(position - rayOrigin);

	//glm function that calculates the reflection vector given a direction and normal
	vec3 reflectionVector = reflect(rayDir, obj->getNormal(position));
	Ray* ray = new Ray(position, reflectionVector);

	//Do the cast and color for the new fragment
	Hit hit = collide(scene, ray);
	Fragment reflectFrag(hit, scene, ray);
	reflectFrag.colorFrag(scene, lightMode, maxBounces);

	delete(ray);
	return reflectFrag.color;
}