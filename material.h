#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vec3.h"

typedef struct hit_record hit_record_t;

typedef enum material_type {
	MATERIAL_LAMBERTIAN,
	MATERIAL_METAL,
	MATERIAL_DIELECTRIC,
} material_type_t;

typedef struct material_lambertian {
	vec3_t albedo;
} material_lambertian_t;

typedef struct material_metal {
	vec3_t albedo;
	float fuzz;
} material_metal_t;

typedef struct material_dielectric {
	float refraction_index;
} material_dielectric_t;

typedef struct material {
	material_type_t type;
	union {
		material_lambertian_t lambertian;
		material_metal_t metal;
		material_dielectric_t dielectric;
	};
} material_t;

void material_lambertian_init(material_t *mat, vec3_t albedo);
void material_metal_init(material_t *mat, vec3_t albedo, float fuzz);
void material_dielectric_init(material_t *mat, float refraction_index);

int material_scatter(material_t *mat, ray_t r, hit_record_t *rec, vec3_t *attenuation, ray_t *scattered);

#endif
