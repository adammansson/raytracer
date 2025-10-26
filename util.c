#include "util.h"
#include <stdlib.h>
#include <time.h>

double randomd(double min, double max)
{
	return min + (max - min) * (((double)rand()) / (RAND_MAX + 1.0));
}

double degrees_to_radians(double degrees)
{
	return degrees * 3.14159265358979 / 180.0;
}
