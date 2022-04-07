#include <math.h>

double sq(double x) {
	return x * x;
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

int solve_depressed_cubic(double c[4], double x[3]) {
	if (c[2] != 0) return -1;
	
}

int solve_cubic(double c[4], double x[3]) {
	if (c[3] == 0) {
		return solve_quadratic(c[2], c[1], c[0], x);
	}

	return 0;
}

