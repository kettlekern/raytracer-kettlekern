#include "RayManager.h"

std::vector<Ray*> genRays(int width, int height, glm::vec3 origin, Scene* scene) {
	std::vector<Ray*> rays;
	float focalLength = 1;
	Camera camera = scene->getCamera();
	glm::vec3 W = focalLength * normalize(camera.lookat - camera.location);
	for (int i = 0; i < width; i++) {
		//index + sub-pixel offset / image width - (bring image to (-.5,.5) from (0,1))
		float Us = i + .5f / width - .5f;
		glm::vec3 U = Us * camera.right;
		for (int j = 0; j < height; j++) {
			float Vs = j + .5f / height - .5f;
			glm::vec3 direction = 
					U + 
					Vs * camera.up +
					W;
			rays.push_back(new Ray(origin, direction));
		}
	}

	return rays;
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