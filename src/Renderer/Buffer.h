#pragma once
#ifndef __BUFFER_473_
#define __BUFFER_473_
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Fragment.h"

class Buffer {
	std::vector<Fragment *> fragBuffer;
	int width, height;

public:
	//TODO: rule of 5 this
	Buffer(int width, int height) : width(width), height(height) {
	}

	Fragment* getFragment(int width, int height);
	void setFragment(int width, int height, Fragment* frag);
	void push_back(Fragment* frag);

	//This returns a malloc'ed array with the colors of the fragments in order. Must call free on the pointer this returns after it is used.
	unsigned char* toArray();
};



#endif