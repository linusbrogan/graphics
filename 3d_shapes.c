#include "FPToolkit.c"
#include "M3d_matrix_tools.c"
#include "2d_shape_functions.c"
#include "3d_shape_functions.c"

#define NUM_PTS 500
#define WINDOW_SIZE 800
#define HALF_ANGLE (M_PI / 6)
#define H (tan(HALF_ANGLE))


double Z_BUFFER[WINDOW_SIZE][WINDOW_SIZE];

void initialize_z_buffer(int size) {
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			Z_BUFFER[x][y] = 1e50;
		}
	}
}

int project(double x, double z) {
	double x_bar = x / z;
	double x_prime = (x_bar / H + 1) * WINDOW_SIZE / 2;
	return round(x_prime);
}

void graph_3d(
	double (*f_x)(double, double),
	double (*f_y)(double, double),
	double (*f_z)(double, double),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double T[4][4]
) {
	double du = (u_end - u_start) / NUM_PTS;
	double dv = (v_end - v_start) / NUM_PTS;
	for (double u = u_start; u <= u_end; u += du) {
		for (double v = v_start; v <= v_end; v += dv) {
			double point[3] = {
				f_x(u, v),
				f_y(u, v),
				f_z(u, v)
			};
			M3d_mat_mult_pt(point, T, point);
			double z = point[2];
			int x = project(point[0], z);
			int y = project(point[1], z);
			if (x >= 0 && x < WINDOW_SIZE && y >= 0 && y < WINDOW_SIZE) {
				if (z > 0 && z < Z_BUFFER[x][y]) {
					Z_BUFFER[x][y] = z;
					G_point(x, y);
				}
			}
		}
	}
}

int main() {
	// Initialize screen
	initialize_z_buffer(WINDOW_SIZE);
	G_init_graphics(WINDOW_SIZE, WINDOW_SIZE);
	G_rgb(0, 0, 0);
	G_clear();

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];

	// Move into world space
	T_n = 0;
	T_type[T_n] = TZ;	T_param[T_n] = 1.0;	T_n++;
	T_type[T_n] = RX;	T_param[T_n] = 30;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 1;	T_n++;
	double A[4][4];
	double A_i[4][4];
	M3d_make_movement_sequence_matrix(A, A_i, T_n, T_type, T_param);

	// Make view matrix
	double view[4][4];
	double view_i[4][4];
	double eye[3] = {0, 0, -10};
	double coi[3] = {0, 0, 2};
	double up[3] = {0, 1, 1};
	M3d_view(view, view_i, eye, coi, up);


	G_rgb(1, 0, 0);
	graph_3d(sphere_x, sphere_y, sphere_z, M_PI / 4, 1.75 * M_PI, -M_PI / 3, M_PI / 3, A);
	G_wait_key();

	M3d_mat_mult(A, view, A_i);
	G_rgb(0, 1, 0);
	graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, M_PI, -3, 1.5, A);

	while (1)
		if (G_wait_key() == 'q')
			return 0;

	return 0;
}
