#include <math.h>

double identity(double x) {
	return x;
}

double sgn(double x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

double (*circle_x)(double) = cos;
double (*circle_y)(double) = sin;

void d_circle(double x, double y, double d[2]) {
	d[0] = 2 * x;
	d[1] = 2 * y;
}

double (*sum4_x)(double) = identity;
double sum4_y(double u) {
	return pow(1 - u * u * u * u, 0.25);
}

void d_sum4(double x, double y, double d[2]) {
	d[0] = 4 * pow(x, 3);
	d[1] = 4 * pow(y, 3);
}

double square_x(double u) {
	if (u >= 0 && u < 2)
		return 1 - u;
	if (u >= 2 && u <= 4)
		return u - 3;

	return 0;
}

double square_y(double u) {
	if (u >= 0 && u < 1)
		return u;
	if (u >= 1 && u < 3)
		return 2 - u;
	if (u >= 3 && u <= 4)
		return u - 4;

	return 0;
}

void d_square(double x, double y, double d[2]) {
	d[0] = sgn(x);
	d[1] = sgn(y);
}

double square2_x(double u) {
	return sgn(cos(u)) * pow(cos(u), 2);
}

double square2_y(double u) {
	return sgn(sin(u)) * pow(sin(u), 2);
}

double astroid_x(double u) {
	return sgn(cos(u)) * pow(cos(u), 4);
}

double astroid_y(double u) {
	return sgn(sin(u)) * pow(sin(u), 4);
}

void d_astroid(double x, double y, double d[2]) {
	d[0] = 0.5 * pow(fabs(x), -0.5) * sgn(x);
	d[1] = 0.5 * pow(fabs(y), -0.5) * sgn(y);
}

double (*hyperbola_x)(double) = cosh;
double (*hyperbola_y)(double) = sinh;

void d_hyperbola(double x, double y, double d[2]) {
	d[0] = 2 * x;
	d[1] = -2 * y;
}

double (*parabola_x)(double) = identity;
double parabola_y(double u) {
	return u * u;
}

void d_parabola(double x, double y, double d[2]) {
	d[0] = 2 * x;
	d[1] = -1;
}

double lemon_x(double u) {
	return pow(cos(u), 3);
}

double (*lemon_y)(double) = sin;

void d_lemon(double x, double y, double d[2]) {
	d[0] = 2 * x;
	d[1] = 6 * y * pow(1 - y * y, 2);
}

double brachistochrone_x(double u) {
	return u - sin(u);
}

double brachistochrone_y(double u) {
	return 1 - cos(u);
}

double line_x(double u) {
	return u;
}

double line_y(double u) {
	return 0;
}

void d_line(double x, double y, double d[2]) {
	d[0] = 0;
	d[1] = 1;
}

int circle_xyz(int step, int resolution, double xyz[3]) {
	double t = step * 2 * M_PI / resolution;
	xyz[0] = circle_x(t);
	xyz[1] = circle_y(t);
	xyz[2] = 0;
	return 1;
}

int half_hyperbola_xyz(int step, int resolution, double xyz[3]) {
	double t = -1 + step * 2.0 / resolution;
	xyz[0] = hyperbola_x(t);
	xyz[1] = hyperbola_y(t);
	xyz[2] = 0;
	if (fabs(xyz[1]) > 1) return 0;
	return 1;
}

int hyperbola_xyz(int step, int resolution, double xyz[3]) {
	int status = half_hyperbola_xyz(step, resolution, xyz);
	if (step % 2 == 1) xyz[0] *= -1;
	return status;
}

int line_xyz(int step, int resolution, double xyz[3]) {
	double t = -1 + step * 2.0 / resolution;
	xyz[0] = line_x(t);
	xyz[1] = line_y(t);
	xyz[2] = 0;
	if (fabs(xyz[0]) > 1) return 0;
	return 1;
}
