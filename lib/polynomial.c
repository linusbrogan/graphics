#include <math.h>

double sq(double x) {
	return x * x;
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
	// Not a quadratic
	if (a == 0) {
		int n = solve_line(b, c, x);
		if (n == 1) {
			x[1] = x[0];
		}
		return n;
	}

	// Solve
	double root = b * b - 4 * a * c;
	if (root < 0) return 0;
	root = sqrt(root);
	x[0] = (-b - root) / (2 * a);
	x[1] = (-b + root) / (2 * a);
	if (root == 0) return 1;
	return 2;
}
