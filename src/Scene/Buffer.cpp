#include "Buffer.h"

Fragment* Buffer::getFragment(int width, int height) {
	return fragBuffer[width*this->width + height];
}

void Buffer::setFragment(int width, int height, Fragment* frag) {
	fragBuffer[width*this->width + height] = frag;
}

void Buffer::push_back(Fragment* frag) {
	fragBuffer.push_back(frag);
}

unsigned char* Buffer::toArray(){
	//make_shared does not support arrays until c++17 and you need an external library for it then, so we will just manage the memory ourselves for now
	unsigned char* colArray = (unsigned char*)malloc(width*height*3);
	//Could also make a vector with the colors as elements, but this is more efficent.
	for (int i = 0; i < width*height; i++) {
		//This assumes that the color in fragments is scaled from 0-255, may change later.
		colArray[3 * i] = fragBuffer[i]->getColor().r;
		colArray[3 * i + 1] = fragBuffer[i]->getColor().g;
		colArray[3 * i + 2] = fragBuffer[i]->getColor().b;
	}
	return colArray;
}
