#include "Fragment.h"

using namespace glm;


glm::vec3 Fragment::CookTorrance(const std::vector<Light *> & lights) {
	vec3 color = vec3(0, 0, 0);
	for (Light* light : lights) {
		//Change this to use a cook-torrance lighting model instead
		color += BlinnPhongObject(position, obj->getNormal(position), vec3(0, 0, 0), light->color, light->color, cam->location, light->location, light->color, light->shine);
	}
	return color;
}

//This is pretty much shader code for blinn phong and should have similar properties to fragment shader code
glm::vec3 Fragment::BlinnPhongObject(vec3 position, vec3 normal, vec3 ambient, vec3 diffuseColor, vec3 specularColor, vec3 cameraPos, vec3 lightPos, vec3 lightColor, float shine) {
	vec3 viewDir = normalize(cameraPos - position);

	vec3 lightDir = normalize(lightPos - position);
	vec3 H = normalize(lightPos + viewDir);
	//I think this is off by a bit
	float attenuation = clamp(1.0 / pow(length(lightPos - position), 3.0), 0.0, 1.0);

	vec3 diffuse = glm::max(dot(normal, lightDir), 0.0f) * diffuseColor * lightColor;
	vec3 specular = glm::max(specularColor * lightColor * pow(glm::dot(normal, H), shine), 0.0f);

	vec3 lighting = (diffuse + specular) * attenuation;

	return lighting;
}

glm::vec3 Fragment::BlinnPhong(const std::vector<Light *> & lights) {
	vec3 color = vec3(0,0,0);
	for (Light* light : lights) {
		color += BlinnPhongObject(position, obj->getNormal(position), vec3(0, 0, 0), light->color, light->color, cam->location, light->location, light->color, light->shine);
	}
	return color;
}

void Fragment::computeLighting(glm::vec3 (*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights) {
	color = (*lighting)(lights);
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode) {
	switch (lightMode) {
	case BLINN_PHONG:
		color = BlinnPhong(scene->getLights());
		break;
	case COOK_TORRANCE:
		color = CookTorrance(scene->getLights());
		break;
	default:

	}
}
