#include "OpenSimplexNoise.hh"

// Array of gradient values for 2D. They approximate the directions to the
// vertices of a octagon from its center.
// Gradient set 2014-10-06.
const int OSN::Noise<2>::gradients[] = {
	5, 2,   2, 5,  -5, 2,  -2, 5,
	5,-2,   2,-5,  -5,-2,  -2,-5
};

// Array of gradient values for 3D. They approximate the directions to the
// vertices of a rhombicuboctahedron from its center, skewed so that the
// triangular and square facets can be inscribed in circles of the same radius.
// New gradient set 2014-10-06.
const int OSN::Noise<3>::gradients[] = {
	-11, 4, 4,  -4, 11, 4,  -4, 4, 11,   11, 4, 4,   4, 11, 4,   4, 4, 11,
	-11,-4, 4,  -4,-11, 4,  -4,-4, 11,   11,-4, 4,   4,-11, 4,   4,-4, 11,
	-11, 4,-4,  -4, 11,-4,  -4, 4,-11,   11, 4,-4,   4, 11,-4,   4, 4,-11,
	-11,-4,-4,  -4,-11,-4,  -4,-4,-11,   11,-4,-4,   4,-11,-4,   4,-4,-11
};

// Array of gradient values for 4D. They approximate the directions to the
// vertices of a disprismatotesseractihexadecachoron from its center, skewed so that the
// tetrahedral and cubic facets can be inscribed in spheres of the same radius.
// Gradient set 2014-10-06.
const int OSN::Noise<4>::gradients[] = {
	3, 1, 1, 1,   1, 3, 1, 1,   1, 1, 3, 1,   1, 1, 1, 3,
	-3, 1, 1, 1,  -1, 3, 1, 1,  -1, 1, 3, 1,  -1, 1, 1, 3,
	3,-1, 1, 1,   1,-3, 1, 1,   1,-1, 3, 1,   1,-1, 1, 3,
	-3,-1, 1, 1,  -1,-3, 1, 1,  -1,-1, 3, 1,  -1,-1, 1, 3,
	3, 1,-1, 1,   1, 3,-1, 1,   1, 1,-3, 1,   1, 1,-1, 3,
	-3, 1,-1, 1,  -1, 3,-1, 1,  -1, 1,-3, 1,  -1, 1,-1, 3,
	3,-1,-1, 1,   1,-3,-1, 1,   1,-1,-3, 1,   1,-1,-1, 3,
	-3,-1,-1, 1,  -1,-3,-1, 1,  -1,-1,-3, 1,  -1,-1,-1, 3,
	3, 1, 1,-1,   1, 3, 1,-1,   1, 1, 3,-1,   1, 1, 1,-3,
	-3, 1, 1,-1,  -1, 3, 1,-1,  -1, 1, 3,-1,  -1, 1, 1,-3,
	3,-1, 1,-1,   1,-3, 1,-1,   1,-1, 3,-1,   1,-1, 1,-3,
	-3,-1, 1,-1,  -1,-3, 1,-1,  -1,-1, 3,-1,  -1,-1, 1,-3,
	3, 1,-1,-1,   1, 3,-1,-1,   1, 1,-3,-1,   1, 1,-1,-3,
	-3, 1,-1,-1,  -1, 3,-1,-1,  -1, 1,-3,-1,  -1, 1,-1,-3,
	3,-1,-1,-1,   1,-3,-1,-1,   1,-1,-3,-1,   1,-1,-1,-3,
	-3,-1,-1,-1,  -1,-3,-1,-1,  -1,-1,-3,-1,  -1,-1,-1,-3
};