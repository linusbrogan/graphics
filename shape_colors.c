void checkerboard_color(double u, double v, double *rgb) {
	double uu = fmod(u, 2) + 2;
	double vv = fmod(v, 2) + 2;
	int a = fmod(uu, 2) < 1;
	int b = fmod(vv, 2) < 1;
	int c = a != b;
	rgb[0] = c ? 0.7 : 0.3;
	rgb[1] = rgb[0];
	rgb[2] = rgb[0];
}

void (*origin_rgb)(double, double, double *) = checkerboard_color;

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
