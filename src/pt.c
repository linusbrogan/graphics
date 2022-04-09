#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../lib/polynomial.c"

#define T "\t"
#define N "\n"

int eq(double a, double b) {
	// https://www.reddit.com/r/C_Programming/comments/4thsn7/comparing_doubles_in_c/
	//return (fabs(a - b) < (DBL_EPSILON * fabs(a + b)));
	double EPSILON = 1e-10;
	return fabs(a - b) < EPSILON;
}

int true_test() {
	printf(T"True test\n");
	return 1;
}

int false_test() {
	printf(T"False test\n");
	return 0;
}

int smaller_polynomial_test() {
	printf(T"Non-cubic test\n");

	// Check 0 = 0 (all solutions)
	double cs[4] = {0, 0, 0, 0};
	double x[3] = {0, 0, 0};
	int n = solve_cubic(cs, x);
	assert(n == -1);

	// Check 1 = 0 (no solutions)
	cs[0] = 1;
	n = solve_cubic(cs, x);
	assert(n == 0);

	// Check x^2 + 1 = 0 (no solutions)
	cs[2] = 1;
	n = solve_cubic(cs, x);
	assert(n == 0);

	// Check x^2 - 2x + 1 = 0 (one solution)
	cs[1] = -2;
	n = solve_cubic(cs, x);
	assert(n == 1);
	assert(x[0] == 1);

	// Check x^2 - 1 = 0 (two solutions)
	cs[1] = 0;
	cs[0] = -1;
	n = solve_cubic(cs, x);
	assert(n == 2);
	assert(fmin(x[0], x[1]) == -1);
	assert(fmax(x[0], x[1]) == 1);

	return 1;
}

int cbrt_test() {
	printf(T"Cube root polynomial"N);
	double cs[4] = {54, 0, 0, 2};
	double xs[3];
	int n = solve_cubic(cs, xs);
	assert(n == 1);
	printf("%d", eq(xs[0], -3));
}

int del_ferro_jeff_example() {
	printf(T"Del Ferro: Jeff's example"N);
	double cs[4] = {28, -9, 0, 1};
	double xs[3] = {0, 0, 0};
	int n = solve_cubic(cs, xs);
	assert(n == 2);
	assert(xs[0] == -4);
	assert(xs[1] == -4);
	return 1;
}

int del_ferro_playing() {
	printf(T"Del Ferro playing"N);
	double cs[4] = {42, -4, 0, 1};
	double xs[3] = {0, 0, 0};
	int n = solve_cubic(cs, xs);
	assert(n >= 1 && n <= 3);
	for (int i = 0; i < n; i++) {
		double y = evaluate_polynomial(cs, 3, xs[i]);
		assert(eq(y, 0));
	}
	if (n >= 2) {
		assert(eq(xs[0], xs[1]));
	}

	cs[0] = -95;
	cs[1] = 7;
	n = solve_cubic(cs, xs);
	assert(n >= 1 && n <= 3);
	for (int i = 0; i < n; i++) {
		double y = evaluate_polynomial(cs, 3, xs[i]);
		assert(eq(y, 0));
	}
	if (n >= 2) {
		assert(eq(xs[0], xs[1]));
	}

	cs[0] = -18;
	cs[1] = -70;
	n = solve_cubic(cs, xs);
	printf(T"n: %d, %lf, %lf, %lf"N, n, xs[0],xs[1], xs[2]);
	assert(n >= 1 && n <= 3);
	for (int i = 0; i < n; i++) {
		double y = evaluate_polynomial(cs, 3, xs[i]);
		printf("soln: %lf\n", xs[i]);
		assert(eq(y, 0));
	}

	return 1;
}

int (*TEST[])() = {
	true_test,
	smaller_polynomial_test,
	cbrt_test,
	del_ferro_jeff_example,
	del_ferro_playing,
//	false_test,
	NULL
};

int main() {
	for (int i = 0; TEST[i] != NULL; i++) {
		printf("# Running test %d\n", i);
		if (TEST[i]() == 0) {
			printf("->Test %d failed\n", i);
			return 1;
		} else {
			printf("->Test %d passed\n", i);
		}
	}
}
