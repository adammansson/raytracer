#ifndef VEC3_H
#define VEC3_H

#include "util.h"
#include <math.h>
#include <stdio.h>

typedef struct vec3 {
	float x, y, z;
} vec3_t;

static inline float vec3_length_squared(vec3_t u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

static inline float vec3_length(vec3_t u)
{
	return sqrtf(vec3_length_squared(u));
}

static inline vec3_t vec3_scaled(vec3_t u, float t)
{
	return (vec3_t){u.x * t, u.y * t, u.z * t};
}

static vec3_t vec3_normalized(vec3_t u)
{
	float l;

	l = vec3_length(u);

	return (vec3_t){u.x / l, u.y / l, u.z / l};
}

static inline int vec3_near_zero(vec3_t u)
{
	float eps;

	eps = 1e-8;

	return (fabs(u.x) < eps) && (fabs(u.y) < eps) && (fabs(u.z) < eps);
}

static inline vec3_t vec3_add(vec3_t u, vec3_t v)
{
	return (vec3_t){u.x + v.x, u.y + v.y, u.z + v.z};
}

static inline vec3_t vec3_sub(vec3_t u, vec3_t v)
{
	return (vec3_t){u.x - v.x, u.y - v.y, u.z - v.z};
}

static inline float vec3_dot(vec3_t u, vec3_t v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

static vec3_t vec3_mul(vec3_t u, vec3_t v)
{
	return (vec3_t){u.x * v.x, u.y * v.y, u.z * v.z};
}

static vec3_t vec3_cross(vec3_t u, vec3_t v)
{
	return (vec3_t){u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x};
}

static inline vec3_t vec3_reflected(vec3_t u, vec3_t n)
{
	return vec3_sub(u, vec3_scaled(n, 2 * vec3_dot(u, n)));
}

static vec3_t vec3_refracted(vec3_t u, vec3_t n, float etai_over_etat)
{
	float cos_theta;
	vec3_t r_out_perp;
	vec3_t r_out_parallel;

	cos_theta = fmin(vec3_dot(vec3_scaled(u, -1.0), n), 1.0);
	r_out_perp = vec3_scaled(vec3_add(u, vec3_scaled(n, cos_theta)), etai_over_etat);
	r_out_parallel = vec3_scaled(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp))));

	return vec3_add(r_out_perp, r_out_parallel);
}

static vec3_t vec3_random(float min, float max)
{
	return (vec3_t){randomf(min, max), randomf(min, max), randomf(min, max)};
}

static vec3_t vec3_random_unit_vector()
{
	vec3_t p;
	float lensq;

	for (;;) {
		p = vec3_random(-1.0, 1.0);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return vec3_normalized(p);
	}
}

static vec3_t vec3_random_on_hemisphere(vec3_t normal)
{
	vec3_t on_unit_sphere;

	on_unit_sphere = vec3_random_unit_vector();
	if (vec3_dot(on_unit_sphere, normal) > 0.0)
		return on_unit_sphere;
	return vec3_scaled(on_unit_sphere, -1.0);
}

static vec3_t vec3_random_in_unit_disk()
{

	vec3_t p;

	for (;;) {
		p = (vec3_t){randomf(-1.0, 1.0), randomf(-1.0, 1.0), 0.0};
		if (vec3_length_squared(p) < 1.0)
			return p;
	}
}

#endif
