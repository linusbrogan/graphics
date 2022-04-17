#include "lib/polynomial.c"
#include <stdio.h>

void cpf(double complex x) {
	printf("%lf+%lfi", creal(x), cimag(x));
}

void cpn(double complex x) {
	cpf(x);
	printf("\n");
}

int main() {
	double complex a = 1;
	double complex b = 0;
	double complex c = 1;
	double complex x[2];
	int n = csolve_quadratic(a, b, c, x);
	printf("%d solutions: %lf+%lfi and %lf+%lfi\n", n, creal(x[0]), cimag(x[0]), creal(x[1]), cimag(x[1]));

	double cs[4] = {-2, 2, -2, 2};
	double xs[3];
	n = solve_cubic(cs, xs);
	printf("%d solutions: %lf, %lf, %lf\n", n, xs[0], xs[1], xs[2]);

	cs[0] = 4;
	cs[1] = -2;
	cs[2] = -4;
	cs[3] = 2;
	n = solve_cubic(cs, xs);
	printf("%d solutions: %lf, %lf, %lf\n", n, xs[0], xs[1], xs[2]);

}
