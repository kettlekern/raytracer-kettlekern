#include "RayManager.h"

std::vector<Ray*> genRays(int width, int height, Scene* scene) {
	std::vector<Ray*> rays;
	for (int i = 0; i < width; i++) {
		//index + sub-pixel offset / image width - (bring image to (-.5,.5) from (0,1))
		for (int j = 0; j < height; j++) {
			rays.push_back(genRay(width, height, scene, i, j));
		}
	}

	return rays;
}

Ray* genRay(int width, int height, Scene* scene, int i, int j) {
	float focalLength = 1;
	Camera camera = scene->getCamera();
	float Us = i + .5f / width - .5f;
	float Vs = j + .5f / height - .5f;
	glm::vec3 direction =
		Us * camera.right +
		Vs * camera.up +
		focalLength * normalize(camera.lookat - camera.location);
	return new Ray(camera.location, direction);
}

Hit collide(Scene* scene, Ray* ray) {
	Hit hitVal, temphit;
	hitVal.isHit = false;
	for (Object* obj : scene->getObjects()) {
		//Needs more checks
		temphit = obj->collide(ray);
		if (hitVal.isHit = false) {
			//if the hit is in front of the camera
			if (temphit.t > 0) {
				hitVal = temphit;
			}
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