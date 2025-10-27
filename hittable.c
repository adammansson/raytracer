#include "hittable.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

static int sphere_hit(hittable_sphere_t *s, ray_t r, interval_t ival,
					  hit_record_t *rec)
{
	vec3_t oc;
	double a, h, c;
	double d, sqrtd;
	double root;
	vec3_t outward_normal;
	double ia;

	oc = vec3_sub(s->center, r.origin);

	a = vec3_length_squared(r.direction);
	h = vec3_dot(r.direction, oc);
	c = vec3_length_squared(oc) - s->radius * s->radius;

	d = h * h - a * c;
	if (d < 0)
		return 0;

	sqrtd = sqrt(d);
	ia = 1.0 / a;
	root = (h - sqrtd) * ia;
	if (!interval_surrounds(ival, root)) {
		root = (h + sqrtd) * ia;
		if (!interval_surrounds(ival, root))
			return 0;
	}

	rec->t = root;
	rec->p = ray_at(r, rec->t);

	outward_normal = vec3_scaled(vec3_sub(rec->p, s->center), 1.0 / s->radius);
	rec->front_face = vec3_dot(r.direction, outward_normal) < 0.0;
	if (rec->front_face)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_scaled(outward_normal, -1.0);
	rec->mat = &s->mat;

	return 1;
}

static int list_hit(hittable_list_t *list, ray_t r, interval_t ival,
					hit_record_t *rec)
{
	unsigned long i;
	hit_record_t temp_rec;
	int hit_anything;
	double closest_so_far;

	hit_anything = 0;
	closest_so_far = ival.max;
	for (i = 0; i < list->length; ++i) {
		if (hittable_hit(&list->objects[i], r,
						 (interval_t){ival.min, closest_so_far}, &temp_rec)) {
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}

	return hit_anything;
}

void hittable_sphere_init(hittable_t *h, vec3_t center, double radius,
						  material_t *mat)
{
	h->type = HITTABLE_SPHERE;
	h->sphere.center = center;
	h->sphere.radius = radius;
	h->sphere.mat = *mat;
}

void hittable_list_init(hittable_t *h)
{
	h->type = HITTABLE_LIST;
	h->list.length = 0;
	h->list.objects = NULL;
}

void hittable_list_add(hittable_list_t *list, hittable_t *object)
{
	list->objects =
		realloc(list->objects, (list->length + 1) * sizeof(hittable_t));
	list->objects[list->length] = *object;
	list->length = list->length + 1;
	// printf("LENGTH IS %lu\n", list->length);
}

void hittable_list_free(hittable_list_t *list)
{
	free(list->objects);
	list->length = 0;
}

int hittable_hit(hittable_t *h, ray_t r, interval_t ival, hit_record_t *rec)
{
	switch (h->type) {
	case HITTABLE_SPHERE:
		return sphere_hit(&h->sphere, r, ival, rec);
	case HITTABLE_LIST:
		return list_hit(&h->list, r, ival, rec);
	default:
		assert(0 && "unknown hittable");
	}

	return 0;
}
