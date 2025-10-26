#ifndef VEC3_H
#define VEC3_H

#include <stdio.h>

typedef struct vec3 {
	double x, y, z;
} vec3_t;

vec3_t vec3_new(double x, double y, double z);
vec3_t vec3_random(double min, double max);
vec3_t vec3_random_unit_vector();
vec3_t vec3_random_on_hemisphere(vec3_t normal);
vec3_t vec3_random_in_unit_disk();

double vec3_length_squared(vec3_t u);
double vec3_length(vec3_t u);
vec3_t vec3_scaled(vec3_t u, double t);
vec3_t vec3_normalized(vec3_t u);
vec3_t vec3_reflected(vec3_t u, vec3_t n);
vec3_t vec3_refracted(vec3_t u, vec3_t n, double etai_over_etat);
int vec3_near_zero(vec3_t u);

vec3_t vec3_add(vec3_t u, vec3_t v);
vec3_t vec3_sub(vec3_t u, vec3_t v);
double vec3_dot(vec3_t u, vec3_t v);
vec3_t vec3_mul(vec3_t u, vec3_t v);
vec3_t vec3_cross(vec3_t u, vec3_t v);

#endif

