#include <math.h>
#include <graphics.h>
#include <rt_utils.h>

enum object_type {
	OBJ_SPHERE = 0,
	OBJ_CYLINDER,
	OBJ_PLANE,
	OBJ_HYPERBOLOID,
	OBJ_SS_RING,
	OBJ_COUNT
};

// atan2 but with outputs in [0, tau)
double atanp(double y, double x) {
	double theta = atan2(y, x);
	if (theta < 0) theta += 2 * M_PI;
	return theta;
}

// Sphere: x^2 + y^2 + z^2 = 1
void d_sphere(double p[3], double d[3], void *params) {
	d[_X] = 2 * p[_X];
	d[_Y] = 2 * p[_Y];
	d[_Z] = 2 * p[_Z];
}

double solve_sphere_intersection(double E[3], double D[3], void *params) {
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

// x(u, v) = cos(u) * cos(v)
// y(u, v) = sin(u) * cos(v);
// z(u, v) = sin(v);
// u in [0, tau)
// v in [-pi / 2, pi / 2]
void reverse_parametrize_sphere(double xyz[3], double uv[2], void *params) {
	uv[_X] = atanp(xyz[_Y], xyz[_X]) / TAU;
	uv[_Y] = asin(xyz[_Z]) / M_PI + 0.5;
}

// Cylinder: x^2 + y^2 = 1
void d_cylinder(double p[3], double d[3], void *params) {
	d[_X] = 2 * p[_X];
	d[_Y] = 2 * p[_Y];
	d[_Z] = 0;
}

double solve_cylinder_intersection(double E[3], double D[3], void *params) {
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

// x(u, v) = cos(u)
// y(u, v) = sin(u)
// z(u, v) = z
// u in [0, tau)
// v in [-1, 1]
void reverse_parametrize_cylinder(double xyz[3], double uv[2], void *params) {
	uv[_X] = atanp(xyz[_Y], xyz[_X]) / TAU;
	uv[_Y] = (xyz[_Z] + 1) / 2;
}

// Plane: z = 0
void d_plane(double p[3], double d[3], void *params) {
	d[_X] = 0;
	d[_Y] = 0;
	d[_Z] = 1;
}

double solve_plane_intersection(double E[3], double D[3], void *params) {
	if (D[_Z] == 0) return -1;
	double t = -E[_Z] / D[_Z];
	double x = E[_X] + t * D[_X];
	double y = E[_Y] + t * D[_Y];
	if (fabs(x) > 1 || fabs(y) > 1) return -1;
	return t;
}

// x(u, v) = u
// y(u, v) = v
// z(u, v) = 0
// u, v in [-1, 1]
void reverse_parametrize_plane(double xyz[3], double uv[2], void *params) {
	uv[_X] = (xyz[_X] + 1) / 2;
	uv[_Y] = (xyz[_Y] + 1) / 2;
}

// Hyperboloid: x^2 - y^2 + z^2 = 1
void d_hyperboloid(double p[3], double d[3], void *params) {
	d[_X] = 2 * p[_X];
	d[_Y] = -2 * p[_Y];
	d[_Z] = 2 * p[_Z];
}

double solve_hyperboloid_intersection(double E[3], double D[3], void *params) {
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

// x(u, v) = cosh(u) * cos(v)
// y(u, v) = sinh(v)
// z(u, v) = cosh(u) * sin(v)
// u in [-arcsinh(1), arcsinh(1)]
// v in [0, tau)
void reverse_parametrize_hyperboloid(double xyz[3], double uv[2], void *params) {
	uv[_X] = (asinh(xyz[_Y]) / asinh(1) + 1) / 2;
	uv[_Y] = atanp(xyz[_Z], xyz[_X]) / TAU;
}

// Space station ring: z = 0
const double ss_ring_R = 1 - 0.2 / sqrt(2);
const double ss_ring_r = 0.4 / sqrt(2);

double solve_ss_ring_intersection(double E[3], double D[3], void *params) {
	double R = ss_ring_R;
	double r = ss_ring_r;
	if (params != NULL) {
		double *rs = params;
		R = rs[0];
		r = rs[1];
	}
	if (D[_Z] == 0) return -1;
	double t = -E[_Z] / D[_Z];
	double x = E[_X] + t * D[_X];
	double y = E[_Y] + t * D[_Y];
	double dr = sqrt(sq(x) + sq(y));
	if (dr < R || dr >  R + r) return -1;
	return t;
}

// x(u, v) = (R + ru) * cos(v)
// y(u, v) = (R + ru) * sin(v)
// z(u, v) = 0
// u in [0, 1]
// v in [0, tau)
void reverse_parametrize_ss_ring(double xyz[3], double uv[2], void *params) {
	double R = ss_ring_R;
	double r = ss_ring_r;
	if (params != NULL) {
		double *rs = params;
		R = rs[0];
		r = rs[1];
	}
	uv[_X] = (sqrt(sq(xyz[_X]) + sq(xyz[_Y])) - R) / r;
	uv[_Y] = atanp(xyz[_Y], xyz[_X]) / TAU;
}

void (*gradient[OBJ_COUNT])(double[3], double[3], void *) = {
	d_sphere,
	d_cylinder,
	d_plane,
	d_hyperboloid,
	d_plane
};

double (*solve_ray_intersection[OBJ_COUNT])(double[3], double[3], void *) = {
	solve_sphere_intersection,
	solve_cylinder_intersection,
	solve_plane_intersection,
	solve_hyperboloid_intersection,
	solve_ss_ring_intersection
};

void (*reverse_parametrize[OBJ_COUNT])(double[3], double[2], void *) = {
	reverse_parametrize_sphere,
	reverse_parametrize_cylinder,
	reverse_parametrize_plane,
	reverse_parametrize_hyperboloid,
	reverse_parametrize_ss_ring
};
