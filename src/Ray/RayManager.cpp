#include "RayManager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"
#include "../Renderer/Buffer.h"

std::vector<Ray*> genRays(int width, int height, Scene* scene) {
	std::vector<Ray*> rays;
	//The image output we use tracks positions from top left to bottom right so the rays need to be in that order
	for (int i = height - 1; i >= 0; --i) {
		//index + sub-pixel offset / image width - (bring image to (-.5,.5) from (0,1))
		for (int j = 0; j < width; j++) {
			rays.push_back(genRay(width, height, scene, j, i));
		}
	}

	return rays;
}

Ray* genRay(int width, int height, Scene* scene, int i, int j) {
	float focalLength = 1;
	Camera camera = scene->getCamera();
	float Us = (i + .5f) / width - .5f;
	float Vs = (j + .5f) / height - .5f;
	glm::vec3 direction =
		Us * camera.right +
		Vs * camera.up -
		focalLength * normalize(camera.location - camera.lookat);
	return new Ray(camera.location, normalize(direction));
}

Hit setHit(float t, Object* obj, Ray* ray) {
	Hit hitVal;
	hitVal.isHit = (t > 0);
	if (hitVal.isHit) {
		hitVal.t = t;
		hitVal.color = obj->getColor();
		hitVal.obj = obj;
		hitVal.objType = obj->getName();
		hitVal.mat = obj->getMaterial();
		hitVal.position = ray->origin + t * ray->direction;
	}
	return hitVal;
}

Hit collide(Scene* scene, Ray* ray) {
	Hit hitVal, temphit;
	hitVal.isHit = false;
	float t;
	for (Object* obj : scene->getObjects()) {
		//Needs more checks
		t = obj->collide(ray);
		temphit = setHit(t, obj, ray);
		if (!hitVal.isHit) {
			hitVal = temphit;
		}
		else if (temphit.isHit) {
			//if the hit is in front of the camera and closer than previous
			if (temphit.t > 0 && temphit.t < hitVal.t) {
				hitVal = temphit;
			}
		}
	}
	return hitVal;
}

void castRays(int width, int height, Scene* scene) {
	std::vector<unsigned char> output;
	std::vector<Ray*> rays = genRays(width, height, scene);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//Cast the ray into the sceen
			Hit val = collide(scene, rays[i * height + j]);
			//if there is a hit, set the pixel color to the value of the object hit
			if (val.isHit) {
				output.push_back((char)(val.color.r * 255));
				output.push_back((char)(val.color.g * 255));
				output.push_back((char)(val.color.b * 255));
			}//Otherwise set the color to black
			else {
				output.push_back('\0');
				output.push_back('\0');
				output.push_back('\0');
			}
		}
	}
	//Draw the image	
	stbi_write_png("output.png", width, height, 3, output.data(), sizeof(char) * 3 * width);
}

void renderScene(int width, int height, Scene* scene, bool useCookTorrance) {
	std::vector<Ray*> rays = genRays(width, height, scene);
	//Create a buffer that holds the fragments
	Buffer fragBuf(width, height);
	Fragment* frag;
	auto lightMode = BLINN_PHONG;
	if (useCookTorrance) {
		lightMode = COOK_TORRANCE;
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//Cast the ray into the sceen
			int rayIndex = i * height + j;
			//Fragment is a more robust hit object, mostly a wrapper for old code
			Hit val = collide(scene, rays[rayIndex]);
			frag = new Fragment(val, scene, rays[rayIndex]);
			frag->colorFrag(scene, lightMode);
			fragBuf.push_back(frag);
		}
	}
	//Draw the image	
	stbi_write_png("output.png", width, height, 3, fragBuf.toArray(), sizeof(char) * 3 * width);
}
