#include <stdio.h>
//#include <rt_shapes.h>
#include <rt_utils.h>

#define FOR3 for (int i = 0 ; i < 3; i++)

int main() {
	setbuf(stdout, NULL);

	double AM[3][4] = {
		{3, 7, 2, 25},
		{1, 3, 1, 11},
		{2, 1, 5, 24}
	};

	double R1[4] = {3, 7, 2, 25};
	double R2[4] = {1, 3, 1, 11};
	double R3[4] = {2, 1, 5, 24};

	double *BM[3] = {R1, R2, R3};

	double x[3];
	int n = solve_3x3_system(AM, x);
	//n = solve_3x3_system(BM, x);


	printf("# Solutions: %d\n", n);
	FOR3 printf("x[%d] = %lf\n", i, x[i]);


	FOR3 printf("%d\n", i);
	FOR3 {
		printf("%d\n", 2 * i);
		printf("%d\n", 3 * i);
	}
}
