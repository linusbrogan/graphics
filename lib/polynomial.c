#include <complex.h>
#include <math.h>
#include <graphics.h>

double sq(double x) {
	return x * x;
}

double cu(double x) {
	return pow(x, 3);
}

double qu(double x) {
	return pow(x, 4);
}

double complex ccbrt(double complex z) {
	double r = cabs(z);
	double theta = carg(z);
	return cbrt(r) * cexp(I * theta / 3);
}

int solve_line(double a, double b, double *x) {
	if (a == 0) {
		if (b == 0) return -1;
		return 0;
	}
	*x = -b / a;
	return 1;
}

int csolve_line(double complex a, double complex b, double complex *x) {
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

int csolve_quadratic(double complex a, double complex b, double complex c, double complex x[2]) {
	// Not a quadratic
	if (a == 0) {
		int n = csolve_line(b, c, x);
		if (n == 1) {
			x[1] = x[0];
		}
		return n;
	}

	// Solve
	double complex root = csqrt(b * b - 4 * a * c);
	x[0] = (-b - root) / (2 * a);
	x[1] = (-b + root) / (2 * a);
	if (root == 0) return 1;
	return 2;
}

// Solve y^3 + by + c = 0 by del Ferro's method
int solve_depressed_cubic(double b, double c, double y[3]) {
	// Cube root
	if (b == 0) {
		y[0] = cbrt(-c);
		return 1;
	}

	// Construct quadratic coefficients
	double A = 1;
	double B = c;
	double C = -cu(b) / 27;
	double root = B * B - 4 * A * C;

	// Find a real root, avoid complex numbers if possible
	if (root >= 0) {
		double w = (-B - sqrt(root)) / (2 * A);
		double z = cbrt(w);
		y[0] = z - b / (3 * z);
	} else {
		double complex w = (-B - csqrt(root)) / (2 * A);
		double complex z = ccbrt(w);
		y[0] = creal(z - b / (3 * z));
	}

	// Reduce to quadratic and find remaining real roots
	int n = solve_quadratic(1, y[0], b + sq(y[0]), y + 1);
	return n + 1;
}

int solve_cubic(double cs[4], double x[3]) {
	// Not a cubic
	if (cs[3] == 0) {
		return solve_quadratic(cs[2], cs[1], cs[0], x);
	}

	// Cube root
	if (cs[1] == 0 && cs[2] == 0) {
		x[0] = cbrt(-cs[0] / cs[3]);
		return 1;
	}

	// Normalize coefficients
	double b = cs[2] / cs[3];
	double c = cs[1] / cs[3];
	double d = cs[0] / cs[3];

	// Construct depressed coefficients
	double bb = -sq(b) / 3 + c;
	double cc = 2 * cu(b) / 27 - b * c / 3 + d;

	// Solve
	double y[3];
	int n = solve_depressed_cubic(bb, cc, y);
	for (int i = 0; i < n; i++) {
		x[i] = y[i] - b / 3;
	}
	return n;
}

// Solve x^4 + ax^2 + bx + c = 0 by Ferrari's method
int solve_depressed_quartic(double a, double b, double c, double xs[4]) {
	// Work with x^4 = ax^2 + bx + c
	a *= -1;
	b *= -1;
	c *= -1;

	// Find y to make perfect squares
	double cs[4] = {
		-sq(b) + 4 * a * c,
		8 * c,
		4 * a,
		8
	};
	double ys[3];
	int n = solve_cubic(cs, ys);
	double y = ys[0];

	// Create quadratic parameters
	double A = a + 2 * y;
	double B = b / (2 * A);
	if (A == 0) B = 0;
	double complex zs[4];

	// Solve (+) quadratic
	double complex bb = csqrt(A);
	double complex cc = y + B * csqrt(A);
	n = csolve_quadratic(1, bb, cc, zs);
	if (n < 0) n = 0;

	// Solve (-) quadratic
	bb = -csqrt(A);
	cc = y - B * csqrt(A);
	n += csolve_quadratic(1, bb, cc, zs + n);

	// Save real roots
	int m = 0;
	for (int i = 0; i < n; i++) {
		double im = cimag(zs[i]);
		if (fabs(im) < EPSILON) {
			xs[m] = creal(zs[i]);
			m++;
		}
	}
	return m;
}

int solve_quartic(double cs[5], double xs[4]) {
	// Not a quartic
	if (cs[4] == 0) return solve_cubic(cs, xs);

	// Normalize coefficients
	double b = cs[3] / cs[4];
	double c = cs[2] / cs[4];
	double d = cs[1] / cs[4];
	double e = cs[0] / cs[4];

	// Construct depressed coefficients
	double aa = -3 * sq(b) / 8 + c;
	double bb = cu(b) / 8 - b * c / 2 + d;
	double cc = -3 * qu(b) / 256 + sq(b) * c / 16 - b * d / 4 + e;

	// Solve
	int n = solve_depressed_quartic(aa, bb, cc, xs);
	for (int i = 0; i < n; i++) {
		xs[i] -= b / 4;
	}
	return n;
}
