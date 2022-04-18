#include <complex.h>
#include <math.h>
#include "cpolynomial.c"

double sq(double x) {
	return x * x;
}

double cu(double x) {
	return pow(x, 3);
}

double qu(double x) {
	return pow(x, 4);
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

	double complex w[2] = {0, 0};
	int n = csolve_quadratic(1, c, -cu(b) / 27, w);
//
	double A = 1;
	double B = c;
	double C = -cu(b) / 27;
	double root = B * B - 4 * A * C;
	if (root >= 0) {
		double w = (-B - sqrt(root)) / (2 * A);
		double z = cbrt(w);
		y[0] = z - b / (3 * z);
	} else {
		double complex w = (-B - csqrt(root)) / (2 * A);
		double complex z = ccbrt(w);
		y[0] = creal(z - b / (3 * z)); // Should always be real
	}
//
	// Reduce to quadratic and find remaining real roots
	n = solve_quadratic(1, y[0], b + sq(y[0]), &(y[1]));
	return n + 1;
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

int solve_depressed_quartic(double a, double b, double c, double xs[4]) {
	// Move terms over in equation
	a *= -1;
	b *= -1;
	c *= -1;

	// Find y to make perfect square
	double cs[4] = {sq(b) + 4 * a * c, 8 * c, 4 * a, 8};
	double ys[3];
	int n = solve_cubic(cs, ys);
	double y = ys[0];

	// Create quadratic parameters
	double A = a + 2 * y; // What if this is zero?
	double B = b / (2 * A);
	double complex zs[4];

	// Solve (+) quadratic
	double complex bb = csqrt(A);
	double complex cc = y + B * bb;
	n = csolve_quadratic(1, bb, cc, zs);

	// Solve (-) quadratic
	bb *= -1;
	cc = y + B * bb;
	n += csolve_quadratic(1, bb, cc, zs + n);

	// Save real roots
	int m = 0;
	for (int i = 0; i < n; i++) {
		double epsilon = 1e-5;
		double im = cimag(zs[i]);
		if (fabs(im) < epsilon) {
			xs[m] = creal(zs[i]);
			m++;
		}
	}
	return m;
}

int solve_quartic(double cs[5], double xs[4]) {
	if (cs[4] == 0) return solve_cubic(cs, xs);

	double b = cs[3] / cs[4];
	double c = cs[2] / cs[4];
	double d = cs[1] / cs[4];
	double e = cs[0] / cs[4];

	double aa = -3 * sq(b) / 8 + c;
	double bb = -cu(b) / 8 - b * c / 2 + d;
	double cc = -3 * qu(b) / 256 + sq(b) * c / 4 - b * d / 4 + e;

	int n = solve_depressed_quartic(aa, bb, cc, xs);
	for (int i = 0; i < n; i++) {
		xs[i] -= b / 4;
	}
	return n;
}
