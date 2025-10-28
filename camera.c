#include "camera.h"
#include "material.h"
#include "util.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

static void initialize(camera_t *camera)
{
	float theta;
	float h;
	float viewport_height;
	float viewport_width;
	vec3_t viewport_u;
	vec3_t viewport_v;
	vec3_t viewport_ul;
	vec3_t u, v, w;
	float defocus_radius;

	camera->aspect_ratio = 16.0 / 9.0;
	camera->image_width = 800;
	camera->image_height = (int)(((float)camera->image_width) / camera->aspect_ratio);
	if (camera->image_height < 1)
		camera->image_height = 1;

	camera->samples_per_pixel = 20;
	camera->max_depth = 50;

	camera->vfov = 20.0;
	camera->lookfrom = (vec3_t){13.0, 2.0, 3.0};
	camera->lookat = (vec3_t){0.0, 0.0, 0.0};
	camera->vup = (vec3_t){0.0, 1.0, 0.0};
	camera->center = camera->lookfrom;

	camera->defocus_angle = 0.6;
	camera->focus_dist = 10.0;

	theta = degrees_to_radians(camera->vfov);
	h = tan(theta / 2.0);
	viewport_height = 2 * h * camera->focus_dist;
	viewport_width = viewport_height * (((float)camera->image_width) / camera->image_height);

	w = vec3_normalized(vec3_sub(camera->lookfrom, camera->lookat));
	u = vec3_normalized(vec3_cross(camera->vup, w));
	v = vec3_cross(w, u);

	viewport_u = vec3_scaled(u, viewport_width);
	viewport_v = vec3_scaled(v, -viewport_height);

	camera->pixel_du = vec3_scaled(viewport_u, 1.0 / camera->image_width);
	camera->pixel_dv = vec3_scaled(viewport_v, 1.0 / camera->image_height);
	viewport_ul =
		vec3_sub(camera->center, vec3_add(vec3_scaled(w, camera->focus_dist),
										  vec3_add(vec3_scaled(viewport_u, 0.5), vec3_scaled(viewport_v, 0.5))));
	camera->pixel00_location = vec3_add(viewport_ul, vec3_scaled(vec3_add(camera->pixel_du, camera->pixel_dv), 0.5));

	defocus_radius = camera->focus_dist * tan(degrees_to_radians(camera->defocus_angle / 2.0));
	camera->defocus_disk_u = vec3_scaled(u, defocus_radius);
	camera->defocus_disk_v = vec3_scaled(v, defocus_radius);

	srand(time(NULL));
}

static vec3_t ray_color(ray_t r, int depth, hittable_t *world)
{
	vec3_t res;
	vec3_t unit_direction;
	float a;
	hit_record_t rec;
	ray_t scattered;
	vec3_t attenuation;

	if (depth <= 0)
		return (vec3_t){0.0, 0.0, 0.0};

	if (hittable_hit(world, r, (interval_t){0.001, 1.0 / 0.0}, &rec)) {
		if (material_scatter(rec.mat, r, &rec, &attenuation, &scattered))
			return vec3_mul(attenuation, ray_color(scattered, depth - 1, world));
		return (vec3_t){0.0, 0.0, 0.0};
	}

	unit_direction = vec3_normalized(r.direction);
	a = 0.5 * (unit_direction.y + 1.0);

	res = vec3_add(vec3_scaled((vec3_t){1.0, 1.0, 1.0}, 1.0 - a), vec3_scaled((vec3_t){0.5, 0.7, 1.0}, a));

	return res;
}

static vec3_t sample_square()
{

	float rx, ry;

	rx = randomf(0.0, 1.0);
	ry = randomf(0.0, 1.0);

	return (vec3_t){rx, ry, 0.0};
}

static vec3_t defocus_disk_sample(camera_t *camera)
{
	vec3_t p;

	p = vec3_random_in_unit_disk();

	return vec3_add(camera->center,
					vec3_add(vec3_scaled(camera->defocus_disk_u, p.x), vec3_scaled(camera->defocus_disk_v, p.y)));
}

static ray_t get_ray(camera_t *camera, int i, int j)
{
	vec3_t offset;
	vec3_t pixel_sample;
	vec3_t ray_origin;
	vec3_t ray_direction;

	offset = sample_square();
	pixel_sample = vec3_add(camera->pixel00_location, vec3_add(vec3_scaled(camera->pixel_du, (i + offset.x)),
															   vec3_scaled(camera->pixel_dv, (j + offset.y))));

	ray_origin = camera->defocus_angle <= 0.0 ? camera->center : defocus_disk_sample(camera);
	ray_direction = vec3_sub(pixel_sample, ray_origin);

	return (ray_t){ray_origin, ray_direction};
}

static float linear_to_gamma(float linear_component)
{
	if (linear_component > 0.0)
		return sqrtf(linear_component);
	return 0;
}

static void write_color(unsigned char *ptr, vec3_t u)
{
	float r, g, b;
	unsigned char rbyte, gbyte, bbyte;
	interval_t intensity;

	r = u.x;
	g = u.y;
	b = u.z;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	intensity = (interval_t){0.000, 0.999};

	rbyte = (unsigned char)(256 * interval_clamp(intensity, r));
	gbyte = (unsigned char)(256 * interval_clamp(intensity, g));
	bbyte = (unsigned char)(256 * interval_clamp(intensity, b));

	*ptr = rbyte;
	*(ptr + 1) = gbyte;
	*(ptr + 2) = bbyte;
}

static vec3_t compute_pixel_color(camera_t *camera, hittable_t *world, int i, int j, double pixel_samples_scale)
{
	vec3_t pixel_color;
	int sample;
	ray_t r;

	pixel_color = (vec3_t){0.0, 0.0, 0.0};
	for (sample = 0; sample < camera->samples_per_pixel; ++sample) {
		r = get_ray(camera, i, j);
		pixel_color = vec3_add(pixel_color, ray_color(r, camera->max_depth, world));
	}

	return vec3_scaled(pixel_color, pixel_samples_scale);
}

void camera_render(camera_t *camera, hittable_t *world)
{
	double pixel_samples_scale;
	int i, j;
	unsigned long buffer_size;
	unsigned char *buffer;
	unsigned long offset;
	FILE *fp;

	initialize(camera);
	pixel_samples_scale = 1.0 / camera->samples_per_pixel;

	buffer_size = 3 * camera->image_width * camera->image_height;
	buffer = malloc(buffer_size);
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// #pragma omp parallel for collapse(2) private(i, j, offset)
	for (j = 0; j < camera->image_height; ++j) {
		for (i = 0; i < camera->image_width; ++i) {
			offset = 3 * (i + j * camera->image_width);
			write_color(buffer + offset, compute_pixel_color(camera, world, i, j, pixel_samples_scale));
		}
	}
	// fprintf(stdout, "\rScanlines remaining: %i\n", camera->image_height - j);
	// fprintf(stdout, "\rDone.\n");

	fp = fopen("image.ppm", "wb");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "P6\n%i %i\n255\n", camera->image_width, camera->image_height);
	fwrite(buffer, 1, buffer_size, fp);

	fclose(fp);
	free(buffer);
}
