#include "Buffer.h"

using namespace std;

void Buffer::push_back(FragmentOutput & frag) {
	fragBuffer.push_back(frag);
}

//This functions leaks memory, but since it is only ever called at the end of a program I don't have to care for now
unsigned char* Buffer::toArray(){
	//make_shared does not support arrays until c++17 and you need an external library for it then, so we will just manage the memory ourselves for now
	unsigned char* colArray = (unsigned char*)malloc(width*height*3);
	//Could also make a vector with the colors as elements, but this is more efficent.
	for (int i = 0; i < width*height; i++) {
		//This assumes that the color in fragments is scaled from 0-1, may change later.
		colArray[3 * i] = (unsigned char) (fragBuffer[i].getColor().r * 255);
		colArray[3 * i + 1] = (unsigned char)(fragBuffer[i].getColor().g * 255);
		colArray[3 * i + 2] = (unsigned char)(fragBuffer[i].getColor().b * 255);
	}
	return colArray;
}
