#include <math.h>
#include <graphics.h>
#include <rt_utils.h>

enum object_type {
	OBJ_SPHERE = 0,
	OBJ_CYLINDER,
	OBJ_PLANE,
	OBJ_HYPERBOLOID,
	OBJ_CONE,
	OBJ_ANNULUS,
	OBJ_TRIANGLE,
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

// Cone: x^2 + y^2 - z^2 = 0
void d_cone(double p[3], double d[3], void *params) {
	d[_X] = 2 * p[_X];
	d[_Y] = 2 * p[_Y];
	d[_Z] = -2 * p[_Z];
}

double solve_cone_intersection(double E[3], double D[3], void *params) {
	double a = sq(D[_X]) + sq(D[_Y]) - sq(D[_Z]);
	double b = 2 * (E[_X] * D[_X] + E[_Y] * D[_Y] - E[_Z] * D[_Z]);
	double c = sq(E[_X]) + sq(E[_Y]) - sq(E[_Z]);

	double t[2] = {-1, -1};
	int n = solve_quadratic(a, b, c, t);

	if (n == 0) return -1;

	// Check for invalid solutions
	for (int solution = 0; solution < 2; solution++) {
		double z = E[2] + t[solution] * D[2];
		if (z < 0 || z > 1 || t[solution] <= 0) {
			t[solution] = -1;
		}
	};

	if (n == 1) return t[0];
	if (t[0] <= 0 && t[1] <= 0) return -1;
	if (t[0] <= 0) return t[1];
	if (t[1] <= 0) return t[0];
	return fmin(t[0], t[1]);
}

// x(u, v) = v * cos(u)
// y(u, v) = v * sin(u)
// z(u, v) = v
// u in [0, tau)
// v in [0, 1]
void reverse_parametrize_cone(double xyz[3], double uv[2], void *params) {
	uv[_X] = atanp(xyz[_Y], xyz[_X]) / TAU;
	uv[_Y] = xyz[_Z];
}

// Annulus: z = 0
// Parameters: inner radius R, annulus width r
// Constraints: R > r > 0
double default_annulus_parameters[2] = {1, 2};
double solve_annulus_intersection(double E[3], double D[3], void *params) {
	if (params == NULL) params = default_annulus_parameters;
	double *parameters = params;
	double R = parameters[0];
	double r = parameters[1];

	if (D[_Z] == 0) return -1;
	double t = -E[_Z] / D[_Z];
	double x = E[_X] + t * D[_X];
	double y = E[_Y] + t * D[_Y];
	double radius = sqrt(sq(x) + sq(y));
	if (radius < R || radius > R + r) return -1;
	return t;
}

// x(u, v) = (R + r * v) * cos(u)
// y(u, v) = (R + r * v) * sin(u)
// z(u, v) = 0
// u in [0, tau)
// v in [0, 1]
void reverse_parametrize_annulus(double xyz[3], double uv[2], void *params) {
	if (params == NULL) params = default_annulus_parameters;
	double *parameters = params;
	double R = parameters[0];
	double r = parameters[1];

	uv[_X] = atanp(xyz[_Y], xyz[_X]) / TAU;
	uv[_Y] = (sqrt(sq(xyz[_X]) + sq(xyz[_Y])) - R) / r;
}

// Triangle with corners A, B, C
// Parameters: coordinates of A, B, C
// P(u, v) = A + u * (B-A) + v * (C-A)
// u in [0, 1]
// v in [0, 1 - u]
double default_triangle_parameters[3 * 3] = {
	0, 0, 0,
	1, 0, 0,
	0, 1, 0
};

void d_triangle(double p[3], double d[3], void *params) {
	if (params == NULL) params = default_triangle_parameters;
	double *parameters = params;
	double *A = parameters;
	double *B = parameters + 3;
	double *C = parameters + 6;
	double AB[3];
	double AC[3];
	for (int i = 0; i < 3; i++) {
		AB[i] = B[i] - A[i];
		AC[i] = C[i] - A[i];
	}

	M3d_x_product(d, AB, AC);
}

double solve_triangle_intersection_uvt(double E[3], double D[3], void *params, double x[3]) {
	if (params == NULL) params = default_triangle_parameters;
	double *parameters = params;
	double *A = parameters;
	double *B = parameters + 3;
	double *C = parameters + 6;
	double AB[3];
	double AC[3];
	for (int i = 0; i < 3; i++) {
		AB[i] = B[i] - A[i];
		AC[i] = C[i] - A[i];
	}

	double AM[3][4];
	for (int i = 0; i < 3; i++) {
		AM[i][0] = AB[i];
		AM[i][1] = AC[i];
		AM[i][2] = -D[i];
		AM[i][3] E[i] - A[i];
	}

	int solutions = solve_3x3_system(AM, x);
	if (solutions <= 0) return -1;
	double u = x[0];
	double v = x[1];
	double t = x[2];
	if (u < 0 || u > 1 || v < 0 || v > 1 - u || t < 0) return -1;
	return t;
}

double solve_triangle_intersection(double E[3], double D[3], void *params) {
	double x[3];
	return solve_triangle_intersection_uvt(E, D, params, x);
}

void reverse_parametrize_triangle(double xyz[3], double uv[2], void *params) {
	double grad[3];
	d_triangle(xyz, grad, params);
	double P[3];
	for (int i = 0; i < 3; i++) {
		P[i] = xyz[i] - grad[i] / 2;
	}
	double uvt[3];
	solve_triangle_intersection_uvt(xyz, grad, params, uvt);
	uv[_X] = uvt[_X];
	uv[_Y] = uvt[_Y];
}

void (*gradient[OBJ_COUNT])(double[3], double[3], void *) = {
	d_sphere,
	d_cylinder,
	d_plane,
	d_hyperboloid,
	d_cone,
	d_plane,
	d_triangle
};

double (*solve_ray_intersection[OBJ_COUNT])(double[3], double[3], void *) = {
	solve_sphere_intersection,
	solve_cylinder_intersection,
	solve_plane_intersection,
	solve_hyperboloid_intersection,
	solve_cone_intersection,
	solve_annulus_intersection,
	solve_triangle_intersection
};

void (*reverse_parametrize[OBJ_COUNT])(double[3], double[2], void *) = {
	reverse_parametrize_sphere,
	reverse_parametrize_cylinder,
	reverse_parametrize_plane,
	reverse_parametrize_hyperboloid,
	reverse_parametrize_cone,
	reverse_parametrize_annulus,
	reverse_parametrize_triangle
};
