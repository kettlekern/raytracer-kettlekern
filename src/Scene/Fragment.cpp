#include "Fragment.h"
#include "../Ray/RayManager.h"
#define EPS 0.05f

using namespace glm;

Fragment::Fragment(const Hit & hit, Scene* scene) {
	if (hit.isHit) {
		position = hit.position;
		//Compute this later
		color = hit.color;
		mat = hit.mat;
		t = hit.t;
		cam = scene->getCamera();
		obj = hit.obj;
	}
	else {
		color = vec3(0, 0, 0);
		obj = nullptr;
	}
	

}

glm::vec3 Fragment::CookTorrance(const std::vector<Light *> & lights) {
	vec3 color = vec3(0, 0, 0);
	if (isHit()) {
		for (Light* light : lights) {
			//Change this to use a cook-torrance lighting model instead
			color += BlinnPhongObject(position, obj->getNormal(position), light->color, light->color, cam.location, light->location, light->color, mat.specular);
		}
	}
	return color;
}

//This is pretty much shader code for blinn phong and should have similar properties to fragment shader code
vec3 Fragment::BlinnPhongObject(vec3 position, vec3 normal, vec3 diffuseColor, vec3 specularColor, vec3 cameraPos, vec3 lightPos, vec3 lightColor, float shine) {
	vec3 viewDir = normalize(cameraPos - position);

	vec3 lightDir = normalize(lightPos - position);
	vec3 H = normalize(lightDir + viewDir);
	//I think this is off by a bit
	float attenuation = 1.0f;//clamp(1.0 / pow(length(lightPos - position), 3.0), 0.0, 1.0);

	vec3 diffuse = glm::max(dot(normal, lightDir), 0.0f) * diffuseColor * lightColor;
	vec3 specular = vec3(0.0f);
	if (shine > 0.0f) {
		specular = specularColor * lightColor * glm::max(pow(glm::dot(normal, H), shine), 0.0f);
	}

	vec3 lighting = (diffuse + specular) * attenuation;

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

vec3 Fragment::BlinnPhong(Scene* scene) {
	vec3 color = vec3(0,0,0);
	vec3 ambient = vec3(0, 0, 0);
	float ambientAmount = 0.3f;
	if (isHit()) {
		ambient = obj->getColor() * ambientAmount;
		for (Light* light : scene->getLights()) {
			if (!inShadow(light, scene)) {
				color += (1 - ambientAmount) * BlinnPhongObject(position, obj->getNormal(position), obj->getColor(), obj->getColor(), cam.location, light->location, clampColor(light->color), mat.roughness == 0 ? 0.0f : pow(2 / mat.roughness, 2) - 2);
			}
		}
	}
	color += ambient;
	color = clampColor(color);
	return color;
}

void Fragment::computeLighting(glm::vec3 (*lighting)(const std::vector<Light *> & lights), const std::vector<Light *> & lights) {
	color = (*lighting)(lights);
}

void Fragment::colorFrag(Scene* scene, LIGHTMODE lightMode) {
	switch (lightMode) {
	case BLINN_PHONG:
		color = BlinnPhong(scene);
		break;
	case COOK_TORRANCE:
		color = CookTorrance(scene->getLights());
		break;
	}
}
