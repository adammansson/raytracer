#include "ray.h"

ray_t ray_new(vec3_t origin, vec3_t direction)
{
	ray_t res;

	res.origin = origin;
	res.direction = direction;

	return res;
}

vec3_t ray_at(ray_t r, double t)
{
	vec3_t res;

	res = vec3_add(r.origin, vec3_scaled(r.direction, t));

	return res;
}
