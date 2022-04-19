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
	printf("Solve x^2 + 1 = 0 complex: should get pm i\n");
	double complex a = 1;
	double complex b = 0;
	double complex c = 1;
	double complex x[2];
	int n = csolve_quadratic(a, b, c, x);
	printf("%d solutions: ", n);
	cpf(x[0]);
	printf(" and ");
	cpn(x[1]);

	printf("Solve 2x^3 - 2x^2 + 2x - 2 = 0, expecting just 1 (and i, -i)\n");
	double cs[4] = {-2, 2, -2, 2};
	double xs[3];
	n = solve_cubic(cs, xs);
	printf("%d solutions: %lf, %lf, %lf\n", n, xs[0], xs[1], xs[2]);

	printf("Solve 2x^3 - 4x^2 - 2x + 4 = 0, expecting: 1, -1, 2\n");
	cs[0] = 4;
	cs[1] = -2;
	cs[2] = -4;
	cs[3] = 2;
	n = solve_cubic(cs, xs);
	printf("%d solutions: %lf, %lf, %lf\n", n, xs[0], xs[1], xs[2]);

}
