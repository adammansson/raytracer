#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

typedef struct vec3 {
	float x, y, z;
} vec3_t;

vec3_t vec3_random(float min, float max);
vec3_t vec3_random_unit_vector();
vec3_t vec3_random_on_hemisphere(vec3_t normal);
vec3_t vec3_random_in_unit_disk();

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

static inline vec3_t vec3_normalized(vec3_t u)
{
	float l;

	l = vec3_length(u);

	return (vec3_t){u.x / l, u.y / l, u.z / l};
}

vec3_t vec3_reflected(vec3_t u, vec3_t n);
vec3_t vec3_refracted(vec3_t u, vec3_t n, float etai_over_etat);
int vec3_near_zero(vec3_t u);

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

vec3_t vec3_mul(vec3_t u, vec3_t v);
vec3_t vec3_cross(vec3_t u, vec3_t v);

#endif
