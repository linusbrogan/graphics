#include <stdio.h>
#include <rt_utils.h>
#include <rt_shapes.h>

#define FOR3 for (int i = 0 ; i < 3; i++)

int main() {
	setbuf(stdout, NULL);

	double AM[3][4] = {
		{3, 7, 2, 25},
		{1, 3, 1, 11},
		{2, 1, 5, 24}
	};
	double x[3];
	int n = solve_3x3_system(AM, x);
	printf("# Solutions: %d\n", n);
	FOR3 printf("x[%d] = %lf\n", i, x[i]);

	double xyz[3] = {0.25, 0.25, 0};
	double uv[3] = {0, 0, 0};
	reverse_parametrize_triangle(xyz, uv, NULL);
	FOR3 printf("uv[%d] = %lf\n", i, uv[i]);
}
