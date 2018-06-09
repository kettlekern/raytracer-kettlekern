#pragma once
#ifndef __BUFFER_473_
#define __BUFFER_473_
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "FragmentOutput.h"

class Buffer {
	std::vector<FragmentOutput> fragBuffer;
	int width, height;

public:
	//TODO: rule of 5 this?
	Buffer(int width, int height) : width(width), height(height) {
	}

	void push_back(FragmentOutput & frag);

	//This returns a malloc'ed array with the colors of the fragments in order. Must call free on the pointer this returns after it is used.
	unsigned char* toArray();
};



#endif