#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "hittable.h"

typedef struct camera {
	double aspect_ratio;
	int image_width;
	int image_height;
	vec3_t center;
	vec3_t pixel00_location;
	vec3_t pixel_du;
	vec3_t pixel_dv;
	int samples_per_pixel;
	int max_depth;
	double vfov;
	vec3_t lookfrom;
	vec3_t lookat;
	vec3_t vup;
	double defocus_angle;
	double focus_dist;
	vec3_t defocus_disk_u;
	vec3_t defocus_disk_v;
} camera_t;

void camera_render(camera_t *camera, hittable_t *world);

#endif
