#include "material.h"
#include "hittable.h"
#include "util.h"
#include <assert.h>
#include <math.h>

void material_lambertian_init(material_t *mat, vec3_t albedo)
{
	mat->type = MATERIAL_LAMBERTIAN;
	mat->lambertian.albedo = albedo;
}

void material_metal_init(material_t *mat, vec3_t albedo, float fuzz)
{
	mat->type = MATERIAL_METAL;
	mat->metal.albedo = albedo;
	if (fuzz > 1.0)
		mat->metal.fuzz = 1.0;
	else
		mat->metal.fuzz = fuzz;
}

void material_dielectric_init(material_t *mat, float refraction_index)
{
	mat->type = MATERIAL_DIELECTRIC;
	mat->dielectric.refraction_index = refraction_index;
}

static int lambertian_scatter(material_lambertian_t *mat, hit_record_t *rec, vec3_t *attenuation, ray_t *scattered)
{
	vec3_t scatter_direction;

	scatter_direction = vec3_add(rec->normal, vec3_random_unit_vector());
	if (vec3_near_zero(scatter_direction))
		scatter_direction = rec->normal;

	*scattered = (ray_t){rec->p, scatter_direction};
	*attenuation = mat->albedo;

	return 1;
}

static int metal_scatter(material_metal_t *mat, ray_t r, hit_record_t *rec, vec3_t *attenuation, ray_t *scattered)
{
	vec3_t reflected;

	reflected = vec3_reflected(r.direction, rec->normal);
	reflected = vec3_add(vec3_normalized(reflected), vec3_scaled(vec3_random_unit_vector(), mat->fuzz));
	*scattered = (ray_t){rec->p, reflected};
	*attenuation = mat->albedo;

	return vec3_dot(scattered->direction, rec->normal) > 0;
}

static float reflectance(float cosine, float refraction_index)
{
	float r0;

	r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
	r0 = r0 * r0;

	return r0 + (1 - r0) * pow(1.0 - cosine, 5);
}

static int dielectric_scatter(material_dielectric_t *mat, ray_t r, hit_record_t *rec, vec3_t *attenuation,
							  ray_t *scattered)
{
	float ri;
	vec3_t unit_direction;
	float cos_theta;
	float sin_theta;
	int cannot_refract;
	vec3_t direction;

	*attenuation = (vec3_t){1.0, 1.0, 1.0};
	ri = rec->front_face ? 1.0 / mat->refraction_index : mat->refraction_index;

	unit_direction = vec3_normalized(r.direction);
	cos_theta = fmin(vec3_dot(vec3_scaled(unit_direction, -1.0), rec->normal), 1.0);
	sin_theta = sqrtf(1.0 - cos_theta * cos_theta);

	cannot_refract = ri * sin_theta > 1.0;
	if (cannot_refract || reflectance(cos_theta, ri) > randomf(0.0, 1.0))
		direction = vec3_reflected(unit_direction, rec->normal);
	else
		direction = vec3_refracted(unit_direction, rec->normal, ri);

	*scattered = (ray_t){rec->p, direction};

	return 1;
}

int material_scatter(material_t *mat, ray_t r, hit_record_t *rec, vec3_t *attenuation, ray_t *scattered)
{
	switch (mat->type) {
	case MATERIAL_LAMBERTIAN:
		return lambertian_scatter(&mat->lambertian, rec, attenuation, scattered);
	case MATERIAL_METAL:
		return metal_scatter(&mat->metal, r, rec, attenuation, scattered);
	case MATERIAL_DIELECTRIC:
		return dielectric_scatter(&mat->dielectric, r, rec, attenuation, scattered);
	default:
		assert(0 && "unknown material");
	}
	return 0;
}
