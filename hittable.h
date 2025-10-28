#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "material.h"
#include "ray.h"

typedef struct hit_record {
	vec3_t p;
	vec3_t normal;
	material_t *mat;
	float t;
	int front_face;
} hit_record_t;

typedef struct hittable hittable_t;

typedef enum hittable_type {
	HITTABLE_SPHERE,
	HITTABLE_LIST,
} hittable_type_t;

typedef struct hittable_sphere {
	vec3_t center;
	float radius;
	material_t mat;
} hittable_sphere_t;

typedef struct hittable_list {
	unsigned long length;
	hittable_t *objects;
} hittable_list_t;

typedef struct hittable {
	hittable_type_t type;
	union {
		hittable_sphere_t sphere;
		hittable_list_t list;
	};
} hittable_t;

void hittable_sphere_init(hittable_t *h, vec3_t center, float radius, material_t *mat);
void hittable_list_init(hittable_t *h);
void hittable_list_add(hittable_list_t *list, hittable_t *object);
void hittable_list_free(hittable_list_t *list);

int hittable_hit(hittable_t *h, ray_t r, interval_t ival, hit_record_t *rec);

#endif
