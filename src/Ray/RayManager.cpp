#include "RayManager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"
#include "../Renderer/Buffer.h"
#include "../Renderer/Volumetric.h"
#include "../Renderer/Simplex Noise/OpenSimplexNoise.hh"

#define FOG_COLOR glm::vec3(0.6f, 0.0f, 0.0f)

std::vector<Ray> genRays(int width, int height, Scene* scene) {
	std::vector<Ray> rays;
	//The image output we use tracks positions from top left to bottom right so the rays need to be in that order
	for (int i = height - 1; i >= 0; --i) {
		//index + sub-pixel offset / image width - (bring image to (-.5,.5) from (0,1))
		for (int j = 0; j < width; j++) {
			rays.push_back(genRay(width, height, scene, j, i));
		}
	}

	return rays;
}

//This is used when creating the camera, it should only be called for the initial ray cast
Ray genRay(int width, int height, Scene* scene, int i, int j) {
	float focalLength = 1;
	Camera camera = scene->getCamera();
	float Us = (i + 0.5f) / width - 0.5f;
	float Vs = (j + 0.5f) / height - 0.5f;
	glm::vec3 direction =
		Us * camera.right +
		Vs * camera.up -
		focalLength * normalize(camera.location - camera.lookat);
	return Ray(camera.location, normalize(direction));
}

Hit setHit(float t, Object* obj, const Ray & ray) {
	Hit hitVal;
	hitVal.isHit = (t > 0);
	if (hitVal.isHit) {
		hitVal.t = t;
		hitVal.color = obj->getColor();
		hitVal.obj = obj;
		hitVal.objType = obj->getName();
		hitVal.mat = obj->getMaterial();
		hitVal.position = ray.origin + t * ray.direction;
	}
	return hitVal;
}

Hit collide(Scene* scene, const Ray & ray) {
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
	std::vector<Ray> rays = genRays(width, height, scene);
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

void renderScene(int width, int height, Scene* scene, Flags flags) {
	std::vector<Ray> rays = genRays(width * flags.superSampleCount, height * flags.superSampleCount, scene);
	//Create a buffer that holds the fragments
	Buffer fragBuf(width, height);
	auto lightMode = BLINN_PHONG;
	Volumetric* fog = new Volumetric(FOG_COLOR, flags.noise);
	if (flags.isAltBRDF) {
		lightMode = COOK_TORRANCE;
	}
	setTime(scene, flags.time);
	fog->setTime(flags.time);
	for (int j = 0; j < height * flags.superSampleCount; j += flags.superSampleCount) {
		for (int i = 0; i < width * flags.superSampleCount; i += flags.superSampleCount) {
			std::vector<Fragment> superSampleBuf;
			for (int ssx = 0; ssx < flags.superSampleCount; ssx++) {
				for (int ssy = 0; ssy < flags.superSampleCount; ssy++) {
					//Cast the ray into the sceen
					int rayIndex = (j + ssx) * width * flags.superSampleCount + (i + ssy);
					//Fragment is a more robust hit object, mostly a wrapper for old code
					Hit val = collide(scene, rays[rayIndex]);
					Fragment frag(val, scene, rays[rayIndex], flags, fog);
					frag.colorFrag(scene, lightMode);
					superSampleBuf.push_back(frag);
				}
			}
			FragmentOutput fragOut(superSampleBuf);
			fragBuf.push_back(fragOut);
		}
	}
	//Draw the image	
	std::string imgName = "output.png";
	if (flags.framesPerSecond * flags.duration > 1) {
		imgName = "output_" + std::to_string(flags.frameNumber) + ".png";
	}
	//This returns an malloc'd array, so free it after calling it
	auto arr = fragBuf.toArray();
	stbi_write_png(imgName.c_str(), width, height, 3, arr, sizeof(char) * 3 * width);
	free(arr);
}

void setTime(Scene * scene, float time)
{
	for (Object* obj : scene->getObjects()) {
		if (obj->isFoggy()) {
			obj->getFogCloud()->setTime(time);
		}
	}
}
