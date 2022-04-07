#include <math.h>

double sq(double x) {
	return x * x;
}

double dot_product(double v[3], double w[3]) {
	double sum = 0;
	for (int i = 0; i < 3; i++) {
		sum += v[i] * w[i];
	}
	return sum;
}

void normalize(double v[3]) {
	double length = sqrt(dot_product(v, v));
	if (length == 0) return;
	for (int i = 0; i < 3; i++) {
		v[i] /= length;
	}
}

void find_reflection_vector(double incident[3], double normal[3], double reflected[3]) {
	double l[3];
	double n[3];
	for (int i = 0; i < 3; i++) {
		l[i] = -incident[i];
		n[i] = normal[i];
	}
	normalize(l);
	normalize(n);
	double scale = 2 * dot_product(l, n);
	for (int i = 0; i < 3; i++) {
		reflected[i] = scale * n[i] - l[i];
	}
}

int solve_line(double a, double b, double *x) {
	if (a == 0) {
		if (b == 0) return -1;
		return 0;
	}
	*x = -b / a;
	return 1;
}

int solve_quadratic(double a, double b, double c, double x[2]) {
	if (a == 0) {
		int n = solve_line(b, c, &(x[0]));
		if (n == 1) {
			x[1] = x[0];
		}
		return n;
	}

	double root = b * b - 4 * a * c;
	if (root < 0) return 0;
	root = sqrt(root);
	x[0] = (-b - root) / (2 * a);
	x[1] = (-b + root) / (2 * a);
	if (root == 0) return 1;
	return 2;
}

void orient_normal(double intersection[3], double normal[3], double eye[3]) {
	// Find vector from intersection point to eye
	double e[3];
	for (int i = 0; i < 3; i++) {
		e[i] = eye[i] - intersection[i];
	}

	// Orient the normal on the same side as the eye
	double dot = dot_product(normal, e);
	if (dot < 0) {
		for (int i = 0; i < 3; i++) {
			normal[i] *= -1;
		}
	}

	normalize(normal);
}

int solve_quartic(double c[5], double t[4]) {
	return 0;
}
