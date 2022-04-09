#include <math.h>

double sq(double x) {
	return x * x;
}

double cu(double x) {
	return pow(x, 3);
}

double isZero(double x) {
	double epsilon = 1e-10;
	return fabs(x) < epsilon;
}

double evaluate_polynomial(double *c, int degree, double x) {
	if (degree < 0) return 0;
	double sum = 0;
	for (int i = 0; i <= degree; i++) {
		sum += c[i] * pow(x, i);
	}
	return sum;
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
		int n = solve_line(b, c, x);
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

// del Ferro's method
// Solve y^3 + by + c = 0
int solve_depressed_cubic(double b, double c, double y[3]) {
	if (b == 0) {
		y[0] = cbrt(-c);
		return 1;
	}

	double w[2] = {0, 0};
	int n = solve_quadratic(1, c, -cu(b) / 27, w);
	if (n == 0) return 0; // What about complex roots?
	double z0 = cbrt(w[0]);
	double z1 = cbrt(w[1]);
	// What if z = 0? Can't divide then!
	// If z = 0, then w = 0, so b = 0 (handled above).
	y[0] = z0 - b / (3 * z0);
	y[1] = z1 - b / (3 * z1);
	return 2;
}

int solve_cubic(double cs[4], double x[3]) {
	// Non-cubic
	if (cs[3] == 0) {
		return solve_quadratic(cs[2], cs[1], cs[0], x);
	}

	// Cube root
	if (cs[1] == 0 && cs[2] == 0) {
		x[0] = cbrt(-cs[0] / cs[3]);
		return 1;
	}

	// Use del Ferro's method
	double b = cs[2] / cs[3];
	double c = cs[1] / cs[3];
	double d = cs[0] / cs[3];
	double bb = -sq(b) / 3 + c;
	double cc = 2 * cu(b) / 27 - b * c / 3 + d;
	double y[3];
	int n = solve_depressed_cubic(bb, cc, y);
	for (int i = 0; i < n; i++) {
		x[i] = y[i] - b / 3;
	}
	return n;
}

