#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "util.h"
#include <math.h>

int main()
{
	hittable_t world;
	material_t m;
	hittable_t s;
	camera_t camera;
	int a, b;
	float choose_mat;
	vec3_t center;
	vec3_t albedo;
	float fuzz;

	hittable_list_init(&world);

	material_lambertian_init(&m, (vec3_t){0.5, 0.5, 0.5});
	hittable_sphere_init(&s, (vec3_t){0.0, -1000.0, 0.0}, 1000.0, &m);
	hittable_list_add(&world.list, &s);

	for (a = -11; a < 11; ++a) {
		for (b = -11; b < 11; ++b) {
			choose_mat = randomf(0.0, 1.0);
			center = (vec3_t){a + 0.9 * randomf(0.0, 1.0), 0.2, b + 0.9 * randomf(0.0, 1.0)};

			if (vec3_length(vec3_sub(center, (vec3_t){4.0, 0.2, 0.0})) > 0.9) {
				if (choose_mat < 0.8) {
					albedo = vec3_mul(vec3_random(0.0, 1.0), vec3_random(0.0, 1.0));
					material_lambertian_init(&m, albedo);
				} else if (choose_mat < 0.95) {
					albedo = vec3_random(0.5, 1.0);
					fuzz = randomf(0.0, 0.5);
					material_metal_init(&m, albedo, fuzz);
				} else {
					material_dielectric_init(&m, 1.5);
				}

				hittable_sphere_init(&s, center, 0.2, &m);
				hittable_list_add(&world.list, &s);
			}
		}
	}

	material_dielectric_init(&m, 1.50);
	hittable_sphere_init(&s, (vec3_t){0.0, 1.0, 0.0}, 1.0, &m);
	hittable_list_add(&world.list, &s);

	material_lambertian_init(&m, (vec3_t){0.4, 0.2, 0.1});
	hittable_sphere_init(&s, (vec3_t){-4.0, 1.0, 0.0}, 1.0, &m);
	hittable_list_add(&world.list, &s);

	material_metal_init(&m, (vec3_t){0.7, 0.6, 0.5}, 0.0);
	hittable_sphere_init(&s, (vec3_t){4.0, 1.0, 0.0}, 1.0, &m);
	hittable_list_add(&world.list, &s);

	camera_render(&camera, &world);

	hittable_list_free(&world.list);

	return 0;
}
