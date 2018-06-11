#pragma once
#ifndef __FLAGS_H_473_
#define __FLAGS_H_473_

struct Flags {
	bool isAltBRDF = false;
	bool useFresnel = false;
	bool useBeers = false;
	bool useSDS = false;
	int superSampleCount = 1;
};

#endif