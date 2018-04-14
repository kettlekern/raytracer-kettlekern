#include "Ray.h"
#include <glm/glm.hpp>
#include <vector>
#include "../Scene/Scene.h"

Hit Ray::collide(Scene* scene) {
	Hit hitVal;
	for (Object* obj : scene->getObjects()) {
		//Needs more checks
		hitVal = collideWith(obj);
	}
	return hitVal;
}

Hit Ray::collideWith(Object* obj) {
	Hit hitVal = obj->collide(this);
	return hitVal;
}
