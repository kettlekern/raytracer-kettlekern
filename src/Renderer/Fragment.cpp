#include "Fragment.h"
#include <limits>
#include "../Ray/RayManager.h"
#include "../Scene/VectorString.h"
#define EPS 0.0005f
#define PI 3.14159265f
#define MAX_BOUNCES 6
#define CLEAR_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

using namespace glm;

Fragment::Fragment(const Hit & hit, Scene* scene, const Ray & ray, const Flags & flags, Volumetric* fog) : flags(flags), fogCloud(fog){
	if (hit.isHit) {
		position = hit.position;
		//Compute this later
		fragColor = hit.color;
		mat = hit.mat;
		t = hit.t;
		obj = hit.obj;
		this->ray = ray;
	}
	else {
		fragColor = CLEAR_COLOR;
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
	return Fo + (1.0f - Fo) * pow(1.0f - cdot(viewDir, H), 5);
}

//This is using GGX
float Fragment::CookTorranceD(float alphasq, const glm::vec3 & normal, const glm::vec3 & H)
{
	float NoH = cdot(normal, H);
	float denom = NoH * NoH * (alphasq - 1.0f) + 1.0f;
	return chiPos(NoH) * alphasq / (PI * denom * denom);
}

//This is using GGX
float Fragment::CookTorranceG(float alphasq, const glm::vec3 & normal, const glm::vec3 & H, const glm::vec3 & VorL)
{
	float VorLoH = cdot(VorL, H);
	int chi = chiPos(VorLoH / cdot(VorL, normal));
	float VoH2 = VorLoH * VorLoH;
	return chi * 2.0f / (1.0f + sqrt(1.0f + alphasq * (1.0f - VoH2) / VoH2));
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
	for (Light* light : scene->getLights()) {
		float shadowMult = 1 - shadowAmount(light, scene);
		if (shadowMult > 0.0f) {
			color += shadowMult * CookTorranceObject(position, obj->getNormal(position, ray.direction), obj->getColor(), obj->getColor(), ray.origin, light->location, light->color, mat.roughness, mat.ior, mat.specular, mat.diffuse);
		}
	}
	color += calcAmbientLight();
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

float Fragment::shadowAmount(Light* light, Scene* scene) {
	float shadow = 0.0f;
	vec3 direction = normalize(light->location - position);
	Ray ray = Ray(position + EPS * direction, direction);
	shadow = shadowAmount(light, scene, ray);
	return shadow;
}

float Fragment::shadowAmount(Light* light, Scene* scene, const Ray & localRay) {
	float shadow = 0.0f;
	Hit hit = collide(scene, localRay);
	//The length of this vector is the number of itterations of the direction it takes to reach the light from the fragment
	//Because the direction vector has length = 1, this value is the t value to the light from the fragment
	float lightT = glm::distance(light->location, hit.position);
	if (hit.isHit && hit.t <= lightT) {
		if (hit.obj->isFoggy()) {
			//Cast a ray thrgouh the object and assume no refraction. 
			Ray shadowRay = Ray(hit.position + EPS * localRay.direction, localRay.direction);
			Hit shadowHit = collide(scene, shadowRay);
			//Check to see if the ray hit an object while in the fog cloud
			if (shadowHit.isHit && shadowHit.t <= lightT) {
				if (shadowHit.obj->isFoggy()) {
					shadow = shadowHit.obj->getFogCloud()->fogGathered(shadowRay.origin, shadowHit.position);
					//Add the extra shadow from the new object to this shadow
					shadow += shadowAmount(light, scene, shadowRay);
					//Shadow has a max value of 1, because then it is blocking all non-ambient light, so enforce that here.
					if (shadow > 1.0f) {
						shadow = 1.0f;
					}
				}
				else {
					shadow = 1.0f;
				}
			}
		}
		else {
			shadow = 1.0f;
		}
	}
	return shadow;
}

vec3 Fragment::BlinnPhong(Scene* scene) 
{
	vec3 color = CLEAR_COLOR;
	vec3 ambient = CLEAR_COLOR;
	for (Light* light : scene->getLights()) {
		float shadowMult = 1 - shadowAmount(light, scene);
		if (shadowMult > 0.0f) {
			color += shadowMult * BlinnPhongObject(position, obj->getNormal(position, ray.direction), obj->getColor(), obj->getColor(), ray.origin, light->location, light->color, mat.roughness == 0 ? 0.0f : 2 / pow(mat.roughness, 2) - 2, mat.diffuse, mat.specular);
		}
	}
	color += calcAmbientLight();
	return color;
}

vec3 Fragment::calcAmbientLight()
{
	return obj->getColor() * mat.ambient;
}

std::string Fragment::toString()
{
	std::string str = "";
	if (isHit()) {
		float ior1 = ray.ior;
		float ior2 = mat.ior;
		//Remove this if/else if objects in other objects is supported
		//May need to account for floating point precision
		if (ior1 == ior2) {
			//In the current system, we can assume this will occur only when exiting an object into air.
			//If moving from an object into air, ior2 = 1.0.
			ior2 = 1.0f;
		}
		vec3 normal = obj->getNormal(position, ray.direction);
		float localAmount = (1 - mat.reflection) * (1 - mat.refraction);
		float reflectionAmount = mat.reflection  * (1 - mat.refraction);
		float refractionAmount = mat.refraction;
		str += "             Ray: " + Parser::vec3ToString(ray.origin) + " -> " + Parser::vec3ToString(ray.direction) + "\n";
		str += "      Hit Object: " + obj->getName() + "\n";
		str += "    Intersection: " + Parser::vec3ToString(position) + " at T = " + formatted_to_string(t) + "\n";
		str += "          Normal: " + Parser::vec3ToString(normal) + "\n";
		str += "     Final Color: " + Parser::vec3ToString(fragColor) + "\n";
		str += "         Ambient: " + Parser::vec3ToString(vec3(mat.ambient)) + "\n";
		str += "         Diffuse: " + Parser::vec3ToString(vec3(mat.diffuse)) + "\n";
		str += "        Specular: " + Parser::vec3ToString(vec3(mat.specular)) + "\n";
		str += "      Reflection: " + Parser::vec3ToString(normalize(reflect(ray.direction, normal))) + "\n";
		str += "      Refraction: " + Parser::vec3ToString(normalize(calcRefractionVector(ray.direction, normal, ray.ior, mat.ior))) + "\n";
		str += "   Contrabutions: " + formatted_to_string(localAmount) + " Local, " + formatted_to_string(reflectionAmount) + " Reflection, " + formatted_to_string(refractionAmount) + " Transmission\n";
		str += "         Ray IOR: " + formatted_to_string(ray.ior) + " Material IOR: " + formatted_to_string(mat.ior) + " IOR2: " + formatted_to_string(ior2) + "\n";
	}
	else {
		str = "No intersection.\n";
	}
	return str;
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode)
{
	colorFrag(scene, lightMode, MAX_BOUNCES, false);
	clampColor();
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode, bool verbose)
{
	colorFrag(scene, lightMode, MAX_BOUNCES, verbose);
	clampColor();
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode, int maxBounces)
{
	colorFrag(scene, lightMode, maxBounces, false);
	clampColor();
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode, int maxBounces, bool verbose)
{
	fragColor = CLEAR_COLOR;
	//Put a limit on the number of times light can bounce in the scene to stop infinite recursion
	if (maxBounces > 0 && isHit()) {
		vec3 localColor = vec3(0.0f);
		vec3 reflectionColor = vec3(0.0f);
		vec3 refractionColor = vec3(0.0f);
		vec3 fogColor = vec3(0.0f);
		float localAmount, reflectionAmount, refractionAmount, fresnelAmount, fogAmount;
		maxBounces--;
		if (flags.useFresnel) {
			fresnelAmount = schlicksApproximation(mat.ior, obj->getNormal(position, ray.direction), -ray.direction);
		}
		else {
			fresnelAmount = 0;
		}
		//ray.inAir is currently never updated, but defaults to true.
		if (flags.useFog && ray.inAir) {
			fogAmount = fogCloud->calcFogAmount(t, fogCloud->calcDensity(ray.origin, ray.direction, t));
			fogColor = fogCloud->fogColorGathered(fogAmount);
			fogColor = clampColor(fogColor);
		}
		else {
			fogAmount = 0.0f;
		}
		localAmount = (1 - mat.reflection) * (1 - mat.refraction) * (1 - fogAmount);
		reflectionAmount = (mat.reflection * (1 - mat.refraction) + mat.refraction * fresnelAmount) * (1 - fogAmount);
		refractionAmount = mat.refraction * (1 - fresnelAmount) * (1 - fogAmount);
		//Calculate the local shading
		localColor = calcLocalColor(scene, lightMode);
		//Calculate shading from refraction
		if (refractionAmount > 0.0f) {
			refractionColor = calcRefractionColor(scene, lightMode, maxBounces, verbose);
			//If there is total internal reflection, do calculate the reflection only once by doing this
			if (refractionColor.x < 0) {
				reflectionAmount += refractionAmount;
				refractionAmount = 0.0f;
			}
		}
		//Calculate shading from reflection
		if (reflectionAmount > 0.0f) {
			reflectionColor = calcReflectionColor(scene, lightMode, maxBounces, verbose);
		}
		fragColor = localColor * localAmount + reflectionColor * reflectionAmount + refractionColor * refractionAmount + fogColor * fogAmount;
		fragColor = clampColor(fragColor);
	}
	else if (flags.useFog) {
		fragColor = fogCloud->getColor();
	}
	if (verbose) {
		std::cout << toString();
	}
}

vec3 Fragment::calcLocalColor(Scene * scene, LIGHTMODE lightMode)
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

vec3 Fragment::calcReflectionColor(Scene * scene, LIGHTMODE lightMode,  int maxBounces, bool verbose)
{
	vec3 normal = obj->getNormal(position, ray.direction);

	//glm function that calculates the reflection vector given a direction and normal
	vec3 reflectionVector = normalize(reflect(ray.direction, normal));
	//Offset the position so the ray does not collide with the current object
	Ray newRay(position + EPS * normal, reflectionVector, ray.ior, true, obj->getID(), &ray);
	newRay.entering = ray.entering;

	//Do the cast and color for the new fragment
	Hit hit = collide(scene, newRay);
	Fragment reflectFrag(hit, scene, newRay, flags, fogCloud);
	reflectFrag.colorFrag(scene, lightMode, maxBounces, verbose);

	return obj->getColor() * reflectFrag.fragColor;
}

vec3 Fragment::calcRefractionColor(Scene * scene, LIGHTMODE lightMode, int maxBounces, bool verbose)
{
	vec3 retColor;
	vec3 normal = obj->getNormal(position, ray.direction);
	float ior1 = ray.ior;
	float ior2 = mat.ior;

	//Remove this if/else if objects in other objects is supported
	//May need to account for floating point precision
	if (ior1 == ior2) {
		//In the current system, we can assume this will occur only when exiting an object into air.
		//If moving from an object into air, ior2 = 1.0.
		ior2 = 1.0f;// findRayIOR(ior1);
	}

	//glm function that calculates the reflection vector given a direction and normal
	vec3 refractionVector = calcRefractionVector(ray.direction, normal, ior1, ior2);

	//If the ray is still in the object, set ior2 back to mat.ior since it is still into object
	if (refractionVector == vec3(0.0f)) {
		//Send back a vector that is normally not possible so we know to add refraction amount to reflection amount
		retColor = vec3(-1.0f);
	}
	else {
		//Offset the position so the ray does not collide with the current object. Offset should be away from the ray's direction which is why it is negative
		Ray newRay(position - EPS * normal, refractionVector, ior2, false, obj->getID(), &ray);
		bool enter = entering(obj->getID(), &newRay);
		newRay.entering = enter;
		//Do the cast and color for the new fragment
		Hit hit = collide(scene, newRay);
		Fragment refractFrag = Fragment(hit, scene, newRay, flags, fogCloud);
		refractFrag.colorFrag(scene, lightMode, maxBounces, verbose);
		retColor = refractFrag.fragColor;
		if (hit.isHit && obj->isFoggy() && newRay.entering){
			float fogAmount = obj->getFogCloud()->fogGathered(newRay.origin, hit.position);
			glm::vec3 fogColor = obj->getColor();
			retColor = fogAmount * fogColor + (1 - fogAmount) * retColor;
		}
		else if(hit.isHit && refractFrag.obj->isFoggy() && !newRay.entering) {
			float fogAmount = refractFrag.obj->getFogCloud()->fogGathered(newRay.origin, hit.position);
			glm::vec3 fogColor = refractFrag.obj->getColor();
			retColor = fogAmount * fogColor + (1 - fogAmount) * retColor;
		}
		else {
			if (enter) {
				if (flags.useBeers) {
					retColor *= beersLaw(glm::length(position - refractFrag.position), obj->getColor());
				}
				else {
					retColor *= obj->getColor();
				}
			}
		}
	}
	return retColor;
}

glm::vec3 Fragment::calcRefractionVector(glm::vec3 direction, glm::vec3 normal, float ior1, float ior2)
{
	vec3 result;
	float n1overn2 = ior1 / ior2;
	float DdotN = glm::dot(direction, normal);
	float radicand = 1 - n1overn2 * n1overn2 * (1 - DdotN * DdotN);
	//If the radicand is < 0, there is total internal reflection, so return a zero vector to account for that.
	if (radicand < 0) {
		result = vec3(0.0f);
	}
	else {
		result = n1overn2 * (direction - DdotN * normal) - normal * glm::sqrt(radicand);
	}
	return normalize(result);
}

//This isn't correct, but its a simplification
bool Fragment::entering(int objID, Ray* newRay)
{
	Ray* temp = newRay;
	while (temp != nullptr) {
		if (!ray.reflection && ray.objID == objID) {
			return false;
		}
		temp = temp->fromRay;
	}
	return true;
}

//This does not support touching objects or overlaped objects
float Fragment::findRayIOR(float ior)
{
	//If the ray is entering, then the object's ior is the ior of the ray
	if (ray.entering) {
		return ior;
	}

	Ray* temp = &ray;
	//This ray must be exiting, so we start the counter at 1
	int exitCount = 1;
	while (exitCount > 0) {
		if (!temp->reflection) {
			if (temp->entering) {
				--exitCount;
			}
			else {
				++exitCount;
			}
		}
		temp = temp->fromRay;
	}
	return temp ? temp->ior : 1.0f;
}

float Fragment::schlicksApproximation(float ior, glm::vec3 normal, glm::vec3 view)
{
	float Fo = pow(ior - 1.0f, 2) / pow(ior + 1.0f, 2);
	return Fo + (1.0f - Fo) * pow(1.0f - glm::abs(cdot(normal, view)), 5);
}

glm::vec3 Fragment::beersLaw(float distance, glm::vec3 color)
{
	glm::vec3 absorbance;
	float alpha = 0.15f;
	float e = 2.71828f;
	absorbance = (vec3(1.0f) - color) * alpha * -distance;
	glm::vec3 amount = glm::vec3(glm::pow(e, absorbance.r), glm::pow(e, absorbance.g), glm::pow(e, absorbance.b));
	return amount;
}

void Fragment::clampColor()
{
	fragColor = clampColor(fragColor);
}
