#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
	vec3_t origin;
	vec3_t direction;
} ray_t;

static inline vec3_t ray_at(ray_t r, float t)
{
	return vec3_add(r.origin, vec3_scaled(r.direction, t));
}

#endif
