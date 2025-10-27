#include "vec3.h"
#include "util.h"
#include <math.h>
#include <stdlib.h>

vec3_t vec3_random(double min, double max)
{
	vec3_t res;

	res.x = randomd(min, max);
	res.y = randomd(min, max);
	res.z = randomd(min, max);

	return res;
}

vec3_t vec3_random_unit_vector()
{
	vec3_t p;
	double lensq;

	for (;;) {
		p = vec3_random(-1.0, 1.0);
		lensq = vec3_length_squared(p);
		if (1e-160 < lensq && lensq <= 1.0)
			return vec3_normalized(p);
	}
}

vec3_t vec3_random_on_hemisphere(vec3_t normal)
{
	vec3_t on_unit_sphere;

	on_unit_sphere = vec3_random_unit_vector();
	if (vec3_dot(on_unit_sphere, normal) > 0.0)
		return on_unit_sphere;
	return vec3_scaled(on_unit_sphere, -1.0);
}

vec3_t vec3_random_in_unit_disk()
{
	vec3_t p;

	for (;;) {
		p = (vec3_t){randomd(-1.0, 1.0), randomd(-1.0, 1.0), 0.0};
		if (vec3_length_squared(p) < 1.0)
			return p;
	}
}

vec3_t vec3_reflected(vec3_t u, vec3_t n)
{
	return vec3_sub(u, vec3_scaled(n, 2 * vec3_dot(u, n)));
}

vec3_t vec3_refracted(vec3_t u, vec3_t n, double etai_over_etat)
{
	double cos_theta;
	vec3_t r_out_perp;
	vec3_t r_out_parallel;

	cos_theta = fmin(vec3_dot(vec3_scaled(u, -1.0), n), 1.0);
	r_out_perp =
		vec3_scaled(vec3_add(u, vec3_scaled(n, cos_theta)), etai_over_etat);
	r_out_parallel =
		vec3_scaled(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp))));

	return vec3_add(r_out_perp, r_out_parallel);
}

int vec3_near_zero(vec3_t u)
{
	double eps;

	eps = 1e-8;

	return (fabs(u.x) < eps) && (fabs(u.y) < eps) && (fabs(u.z) < eps);
}

vec3_t vec3_mul(vec3_t u, vec3_t v)
{
	vec3_t res;

	res.x = u.x * v.x;
	res.y = u.y * v.y;
	res.z = u.z * v.z;

	return res;
}

vec3_t vec3_cross(vec3_t u, vec3_t v)
{
	vec3_t res;

	res.x = u.y * v.z - u.z * v.y;
	res.y = u.z * v.x - u.x * v.z;
	res.z = u.x * v.y - u.y * v.x;

	return res;
}
