#include <math.h>
#include <m3d.h>

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

int solve_quadratic(double a, double b, double c, double x[2]) {
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

// Create a matrix for Cramer's rule
void cramer_copy(int n, double AM[n][n + 1], double M[n][n], int column) {
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			M[r][c] = AM[r][c];
			if (c == column) M[r][c] = AM[r][n];
		}
	}
}

// Solve with Cramer's rule
int solve_3x3_system(double AM[3][4], double x[3]) {
	double M[3][3];
	cramer_copy(3, AM, M, -1);
	double det_M = M3d_det_3x3(M);
	if (det_M == 0) return 0;
	for (int i = 0; i < 3; i++) {
		cramer_copy(3, AM, M, i);
		x[i] = M3d_det_3x3(M) / det_M;
	}
	return 3;
}
