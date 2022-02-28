#include <fpt.h>
#include <m3d.h>
#include <shape_2d.h>

#define NUM_PTS 1000
#define NUM_SHAPES 9
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)

void zero_fill(double *x, int size) {
	for (int i = 0; i < size; i++)
		x[i] = 0;
}

void generate_points(
	double (*f_x)(double),
	double (*f_y)(double),
	double u_start,
	double u_end,
	double *x,
	double *y,
	int points
) {
	double du = (u_end - u_start) / points;
	double u = u_start;
	for (int i = 0; i < points; i++) {
		x[i] = f_x(u);
		y[i] = f_y(u);
		u += du;
	}
}

void plot_points(double *x, double *y, int points) {
	for (int i = 0; i < points; i++)
		G_point(x[i], y[i]);
}

void graph_shape(
	double (*f_x)(double),
	double (*f_y)(double),
	double u_start,
	double u_end,
	double T[4][4]
) {
	double x[NUM_PTS];
	double y[NUM_PTS];
	double z[NUM_PTS];
	zero_fill(z, NUM_PTS);
	generate_points(f_x, f_y, u_start, u_end, x, y, NUM_PTS);
	M3d_mat_mult_points(x, y, z, T, x, y, z, NUM_PTS);
	plot_points(x, y, NUM_PTS);
}

int main() {
	// Initialize screen
	G_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
	G_rgb(0, 0, 0);
	G_clear();

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double A[4][4];
	double A_i[4][4];

	// Graph part of a circle from pi/4 to 3pi/2
	G_rgb(1, 0, 0);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] =	50.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] =	100.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] =	300.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] =	500.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(circle_x, circle_y, 0.25 * M_PI, 1.5 * M_PI, A);
	G_wait_key();

	// Graph sum4 from -1 to 1
	G_rgb(0, 1, 0);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 30.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 30.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 250.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 170.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(sum4_x, sum4_y, -1, 1, A);
	G_wait_key();

	// Graph a square
	G_rgb(0, 0, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 150.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 70.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 500.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 460.0;	T_n++;
 	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(square_x, square_y, 0, 4, A);
	G_wait_key();

	// Graph a square
	G_rgb(0.75, 0.5, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 150.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 70.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 500.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 670.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(square2_x, square2_y, 0, 2 * M_PI, A);
	G_wait_key();

	// Graph an astroid
	G_rgb(1, 1, 0);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 80.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 40.0;	T_n++;
	T_type[T_n] = RZ;	T_param[T_n] = 45.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 130.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 650.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(astroid_x, astroid_y, 0, 2 * M_PI, A);
	G_wait_key();

	// Graph part of a hyperbola from -1 to 1.5
	G_rgb(0, 1, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 70.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 70.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 250.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 150.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(hyperbola_x, hyperbola_y, -1, 1.5, A);
	G_wait_key();

	// Graph part a parabola from -1 to 1
	G_rgb(1, 0, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 150.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 50.0;	T_n++;
	T_type[T_n] = RZ;	T_param[T_n] = 60.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 140.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 200.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(parabola_x, parabola_y, -1, 2, A);
	G_wait_key();

	// Graph a lemon
	G_rgb(1, 1, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 125.0;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 125.0;	T_n++;
	T_type[T_n] = RZ;	T_param[T_n] = 60.0;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 620.0;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 210.0;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(lemon_x, lemon_y, 0, 2 * M_PI, A);
	G_wait_key();

	// Graph 3 cycles of a brachistochrone
	G_rgb(1, 1, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 20;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 20;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 100;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 30;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape(brachistochrone_x, brachistochrone_y, 0, 6 * M_PI, A);
	G_wait_key();

	while (1)
		if (G_wait_key() == 'q')
			return 0;

	return 0;
}
