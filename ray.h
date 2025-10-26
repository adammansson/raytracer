#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
	vec3_t origin;
	vec3_t direction;
} ray_t;

ray_t ray_new(vec3_t origin, vec3_t direction);

vec3_t ray_at(ray_t r, double t);

#endif
