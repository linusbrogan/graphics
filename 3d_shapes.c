#include "FPToolkit.c"
#include "M3d_matrix_tools.c"
#include "2d_shape_functions.c"

#define NUM_PTS 500
#define NUM_SHAPES 9
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)
#define HALF_ANGLE (M_PI / 4)
#define H (tan(HALF_ANGLE))


double Z_BUFFER[WINDOW_WIDTH][WINDOW_HEIGHT];

void initialize_z_buffer(int size) {
/*	if (Z_BUFFER == NULL) {
		Z_BUFFER = malloc(size * size * sizeof(double));
	}
	if (Z_BUFFER == NULL) {
		perror("NO ZBUF");
		exit(1);
	}*/
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Z_BUFFER[i * size + j] = 1e50;
		}
	}
}

void generate_points_3d(
	double (*f_x)(double),
	double (*f_y)(double),
	double (*f_z)(double),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double *x,
	double *y,
	double *z,
	int points
) {
	double du = (u_end - u_start) / points;
	double u = u_start;
	for (int i = 0; i < points; i++) {
		x[i] = f_x(u);
		y[i] = f_y(u);
		z[i] = f_z(u);
		u += du;
	}
}
// zbuffer lab1
void plot_points_3d(double *x, double *y, double *z, int points) {
	for (int i = 0; i < points; i++) {
		if (z[i] < 0) continue;
		double X = x[i] / z[i];
		double Y = y[i] / z[i];
		X *= WINDOW_WIDTH / 2;
		X /= H;
		Y *= WINDOW_WIDTH / 2;
		Y /= H;
		X += WINDOW_WIDTH / 2;
		Y += WINDOW_WIDTH / 2;
		X = round(X);
		Y = round(Y);
		if (Z_BUFFER[((int) X) * WINDOW_WIDTH + (int) Y] > z[i]) {
			Z_BUFFER[((int) X) * WINDOW_WIDTH + (int) Y] = z[i];
			G_point(X, Y);
		}
	}
}

void graph_shape_3d(
	double (*f_x)(double),
	double (*f_y)(double),
	double (*f_z)(double),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double T[4][4]
) {
	double x[NUM_PTS];
	double y[NUM_PTS];
	double z[NUM_PTS];
	generate_points_3d(f_x, f_y, f_z, u_start, u_end, v_start, v_end, x, y, z, NUM_PTS);
	M3d_mat_mult_points(x, y, z, T, x, y, z, NUM_PTS);
	plot_points_3d(x, y, z, NUM_PTS);
}

void 3d_graph(
	double (*f_x)(double, double),
	double (*f_y)(double, double),
	double (*f_z)(double, double),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double T[4][4]
) {
	double du = (u_end - u_start) / 1000;
	double dv = (v_end - v_start) / 1000;
	for (double u = u_start; u <= u_end; u += du) {
		for (double v = v_start; v <= v_end; v += dv) {
			double point[3] = {f_x(u, v), f_y(u, v); f_z(u, v)};
			M3d_mat_mult_pt(point, T, point);
			int x = round(point[0]);
			int y = round(point[1]);
			if (point[3] < Z_BUFFER[x][y] && point[3] > 0) {
				Z_BUFFER[x][y] = point[3];
				G_point(x, y);
			}
		}
	}
}

int main() {
	// Initialize screen
	initialize_z_buffer(WINDOW_WIDTH);
	G_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
	G_rgb(0, 0, 0);
	G_clear();

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double A[4][4];
	double A_i[4][4];

	// Graph 3 cycles of a brachistochrone
	G_rgb(1, 1, 1);
	T_n = 0;
	T_type[T_n] = SX;	T_param[T_n] = 20;	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 20;	T_n++;
	T_type[T_n] = TX;	T_param[T_n] = 100;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 30r;	T_n++;
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);
	graph_shape_3d(brachistochrone_x, brachistochrone_y, identity, 0, 6 * M_PI, 3, 4, A);
	G_wait_key();

	while (1)
		if (G_wait_key() == 'q')
			return 0;

	return 0;
}
