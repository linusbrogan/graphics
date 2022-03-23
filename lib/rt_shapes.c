#include <math.h>
#include <graphics.h>
#include <rt_utils.h>

enum object_type {
	OBJ_SPHERE = 0,
	OBJ_CYLINDER,
	OBJ_PLANE,
	OBJ_HYPERBOLOID,
	OBJ_COUNT
};

// Sphere: x^2 + y^2 + z^2 = 1
void d_sphere(double p[3], double d[3]) {
	d[_X] = 2 * p[_X];
	d[_Y] = 2 * p[_Y];
	d[_Z] = 2 * p[_Z];
}

double solve_sphere_intersection(double E[3], double D[3]) {
	double a = sq(D[_X]) + sq(D[_Y]) + sq(D[_Z]);
	double b = 2 * (E[_X] * D[_X] + E[_Y] * D[_Y] + E[_Z] * D[_Z]);
	double c = sq(E[_X]) + sq(E[_Y]) + sq(E[_Z]) - 1;

	double t[2] = {-1, -1};
	int n = solve_quadratic(a, b, c, t);

	if (n == 0) return -1;
	if (n == 1) return t[0];
	if (t[0] <= 0 && t[1] <= 0) return -1;
	if (t[0] <= 0) return t[1];
	if (t[1] <= 0) return t[0];
	return fmin(t[0], t[1]);
}

// Cylinder: x^2 + y^2 = 1
void d_cylinder(double p[3], double d[3]) {
	d[_X] = 2 * p[_X];
	d[_Y] = 2 * p[_Y];
	d[_Z] = 0;
}

double solve_cylinder_intersection(double E[3], double D[3]) {
	double a = sq(D[_X]) + sq(D[_Y]);
	double b = 2 * (E[_X] * D[_X] + E[_Y] * D[_Y]);
	double c = sq(E[_X]) + sq(E[_Y]) - 1;

	double t[2] = {-1, -1};
	int n = solve_quadratic(a, b, c, t);

	if (n == 0) return -1;

	// Check for invalid solutions
	for (int solution = 0; solution < 2; solution++) {
		double z = E[_Z] + t[solution] * D[_Z];
		if (fabs(z) > 1 || t[solution] <= 0) {
			t[solution] = -1;
		}
	}

	if (n == 1) return t[0];
	if (t[0] <= 0 && t[1] <= 0) return -1;
	if (t[0] <= 0) return t[1];
	if (t[1] <= 0) return t[0];
	return fmin(t[0], t[1]);
}

// Plane: z = 0
void d_plane(double p[3], double d[3]) {
	d[_X] = 0;
	d[_Y] = 0;
	d[_Z] = 1;
}

double solve_plane_intersection(double E[3], double D[3]) {
	if (D[_Z] == 0) return -1;
	double t = -E[_Z] / D[_Z];
	double x = E[_X] + t * D[_X];
	double y = E[_Y] + t * D[_Y];
	if (fabs(x) > 1 || fabs(y) > 1) return -1;
	return t;
}


// Hyperboloid: x^2 - y^2 + z^2 = 1
void d_hyperboloid(double p[3], double d[3]) {
	d[_X] = 2 * p[_X];
	d[_Y] = -2 * p[_Y];
	d[_Z] = 2 * p[_Z];
}

double solve_hyperboloid_intersection(double E[3], double D[3]) {
	double a = sq(D[_X]) - sq(D[_Y]) + sq(D[_Z]);
	double b = 2 * (E[_X] * D[_X] - E[_Y] * D[_Y] + E[_Z] * D[_Z]);
	double c = sq(E[_X]) - sq(E[_Y]) + sq(E[_Z]) - 1;

	double t[2] = {-1, -1};
	int n = solve_quadratic(a, b, c, t);

	if (n == 0) return -1;

	// Check for invalid solutions
	for (int solution = 0; solution < 2; solution++) {
		double y = E[1] + t[solution] * D[1];
		if (fabs(y) > 1 || t[solution] <= 0) {
			t[solution] = -1;
		}
	};

	if (n == 1) return t[0];
	if (t[0] <= 0 && t[1] <= 0) return -1;
	if (t[0] <= 0) return t[1];
	if (t[1] <= 0) return t[0];
	return fmin(t[0], t[1]);
}

void (*gradient[OBJ_COUNT])(double[3], double[3]) = {
	d_sphere,
	d_cylinder,
	d_plane,
	d_hyperboloid
};

double (*solve_ray_intersection[OBJ_COUNT])(double[3], double[3]) = {
	solve_sphere_intersection,
	solve_cylinder_intersection,
	solve_plane_intersection,
	solve_hyperboloid_intersection
};
