#include <stdio.h>
//#include <rt_shapes.h>
#include <rt_utils.h>

#define FOR3 for (int i = 0 ; i < 3; i++)

int main() {
	setbuf(stdout, NULL);

printf("main:"N);
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
printf(T"Gonna solve the system             AM@%p, AM[0]@%p, AM[0][0]@%p"N, AM, AM[0], &AM[0][0]);
for (int r=0;r<3;r++)for(int c=0;c<4;c++)printf(T"Gonna solve the system (r,c)=(%d,%d) AM@%p, AM[r]=%p,@%p, AM[r][c]=%lf,\t@%p"N, r,c, AM, AM[r],&AM[r], AM[r][c],&AM[r][c]);
for (int r=0;r<3;r++)for(int c=0;c<4;c++)printf(T"Gonna solve the system (r,c)=(%d,%d) BM@%p, BM[r]=%p,@%p, BM[r][c]=%lf,\t@%p"N, r,c, BM, BM[r],&BM[r], BM[r][c],&BM[r][c]);
printf(T"Wack = %lf"N, (int *)(AM)[10]);
	int n = solve_3x3_system(&AM[0], x);
printf(T"Solved AM, %d solutions"N, n);
	n = solve_3x3_system(BM, x);
printf(T"Solved BM, %d solutions"N, n);


	printf("# Solutions: %d\n", n);
	FOR3 printf("x[%d] = %lf\n", i, x[i]);


	FOR3 printf("%d\n", i);
	FOR3 {
		printf("%d\n", 2 * i);
		printf("%d\n", 3 * i);
	}
}
