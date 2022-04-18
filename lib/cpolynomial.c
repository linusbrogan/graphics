#include <complex.h>
#include <math.h>

double complex csq(double complex x) {
	return x * x;
}

double complex ccu(double complex x) {
	return cpow(x, 3);
}


double complex ccbrt(double complex z) {
	double r = cabs(z);
	double theta = carg(z);
	return cbrt(r) * cexp(I * theta / 3);
}

double complex cevaluate_polynomial(double complex *c, int degree, double complex x) {
	if (degree < 0) return 0;
	double complex sum = 0;
	for (int i = 0; i <= degree; i++) {
		sum += c[i] * cpow(x, i);
	}
	return sum;
}

int csolve_line(double complex a, double complex b, double complex *x) {
	if (a == 0) {
		if (b == 0) return -1;
		return 0;
	}
	*x = -b / a;
	return 1;
}

int csolve_quadratic(double complex a, double complex b, double complex c, double complex x[2]) {
	if (a == 0) {
		int n = csolve_line(b, c, x);
		if (n == 1) {
			x[1] = x[0];
		}
		return n;
	}

	double complex root = csqrt(b * b - 4 * a * c);
	x[0] = (-b - root) / (2 * a);
	x[1] = (-b + root) / (2 * a);
	if (root == 0) return 1;
	return 2;
}

// del Ferro's method
// Solve y^3 + by + c = 0
int csolve_depressed_cubic(double complex b, double complex c, double complex y[3]) {
	if (b == 0) {
		y[0] = ccbrt(-c);
		return 1;
	}

	double complex w[2] = {0, 0};
	int n = csolve_quadratic(1, c, -ccu(b) / 27, w);
	if (n == 0) return 0;
	double complex z0 = ccbrt(w[0]);
	double complex z1 = ccbrt(w[1]);
	// What if z = 0? Can't divide then!
	// If z = 0, then w = 0, so b = 0 (handled above).
	y[0] = z0 - b / (3 * z0);
	y[1] = z1 - b / (3 * z1);
	return 2;
}

int csolve_cubic(double complex cs[4], double complex x[3]) {
	// Non-cubic
	if (cs[3] == 0) {
		return csolve_quadratic(cs[2], cs[1], cs[0], x);
	}

	// Cube root
	if (cs[1] == 0 && cs[2] == 0) {
		x[0] = ccbrt(-cs[0] / cs[3]);
		return 1;
	}

	// Use del Ferro's method
	double complex b = cs[2] / cs[3];
	double complex c = cs[1] / cs[3];
	double complex d = cs[0] / cs[3];
	double complex bb = -csq(b) / 3 + c;
	double complex cc = 2 * ccu(b) / 27 - b * c / 3 + d;
	double complex y[3];
	int n = csolve_depressed_cubic(bb, cc, y);
	for (int i = 0; i < n; i++) {
		x[i] = y[i] - b / 3;
	}
	return n;
}
