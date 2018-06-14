#pragma once
#ifndef __FLAGS_H_473_
#define __FLAGS_H_473_

struct Flags {
	bool isAltBRDF = false;
	bool useFresnel = false;
	bool useBeers = false;
	bool useSDS = false;
	bool useGI = false;
	bool useFog = false;
	int superSampleCount = 1;
	Flags() {}
	Flags(const Flags & flags) : isAltBRDF(flags.isAltBRDF), useFresnel(flags.useFresnel), useBeers(flags.useBeers), 
		useSDS(flags.useSDS), useGI(flags.useGI), useFog(flags.useFog), superSampleCount(flags.superSampleCount) {}
};

#endif