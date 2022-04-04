#include <math.h>
#include <stdio.h>
#include <lg.h>
#include <textures.h>

#define SPHERE_U_START 0
#define SPHERE_U_END TAU
#define SPHERE_V_END (M_PI / 2)
#define SPHERE_V_START (-SPHERE_V_END)

void clock_map(double u, double v, double *rgb) {
	double x = (u - SPHERE_U_START) / (SPHERE_U_END - SPHERE_U_START);
	double y = (v - SPHERE_V_START) / (SPHERE_V_END - SPHERE_V_START);
	texture_map(TM_CLOCK, x, y, rgb);
}

void earth_map(double u, double v, double *rgb) {
	double x = (u - SPHERE_U_START) / (SPHERE_U_END - SPHERE_U_START);
	double y = (v - SPHERE_V_START) / (SPHERE_V_END - SPHERE_V_START);
	texture_map(TM_EARTH, x, y, rgb);
}

void unit_checkerboard_map(double u, double v, double *rgb) {
	double uu = fmod(u, 2) + 2;
	double vv = fmod(v, 2) + 2;
	int a = fmod(uu, 2) < 1;
	int b = fmod(vv, 2) < 1;
	int c = a != b;
	rgb[0] = c ? 0.7 : 0.3;
	rgb[1] = rgb[0];
	rgb[2] = rgb[0];
}

void (*origin_rgb)(double, double, double *) = unit_checkerboard_map;

void origin_rgb_original(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 0.8;
	rgb[2] = 0;
}

void x_rgb(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 0.2;
	rgb[2] = 0.2;

}

void y_rgb(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 1;
	rgb[2] = 1;
}

void z_rgb(double u, double v, double *rgb) {
	rgb[0] = 0.3;
	rgb[1] = 0.2;
	rgb[2] = 1;

}

void diagonal_rgb(double u, double v, double *rgb) {
	rgb[0] = 0;
	rgb[1] = 0.5;
	rgb[2] = 0.5;
}

void space_grey(double u, double v, double *rgb) {
	rgb[0] = 0.5;
	rgb[1] = 0.5;
	rgb[2] = 0.75;
}
