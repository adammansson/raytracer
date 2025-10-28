#ifndef UTIL_H
#define UTIL_H

#include "vec3.h"
#include <stdlib.h>

static inline float randomf(float min, float max)
{
	return min + (max - min) * (((float)rand()) / (RAND_MAX + 1.0));
}

static inline float degrees_to_radians(float degrees)
{
	return degrees * 3.14159265358979 / 180.0;
}

#endif
