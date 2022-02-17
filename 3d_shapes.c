#include <sys/stat.h>
#include <sys/types.h>
#include "FPToolkit.c"
#include "M3d_matrix_tools.c"
#include "2d_shape_functions.c"
#include "3d_shape_functions.c"

#define NUM_PTS 500
#define WINDOW_SIZE 800
#define HALF_ANGLE (M_PI / 6)
#define H (tan(HALF_ANGLE))
#define OUTPUT_PATH "Graph_3D"

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
	G_init_graphics(WINDOW_SIZE, WINDOW_SIZE);

	// Create frame directory
	mkdir(OUTPUT_PATH, 0777);

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];

	int frame = 0;
	double t = 0;
	while (1) {
		// Reset frame
		initialize_z_buffer(WINDOW_SIZE);
		G_rgb(0, 0, 0);
		G_clear();

		// Configure frame
		t = 0.01 * frame;
		double eye[3] = {15 * cos(2 * M_PI * t), 6 * t, 7 * sin(2 * M_PI * t)};
		double coi[3] = {1, 2, 0.5};
		double up[3] = {eye[0], eye[1] + 1, eye[2]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);


		double _i[4][4];
	///////////
		// Build an origin point
		G_rgb(1, 0.8, 0);
		double M0[4][4];
		double N0[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.25;	T_n++;
		M3d_make_movement_sequence_matrix(M0, _i, T_n, T_type, T_param);
		M3d_mat_mult(N0, view, M0);
		graph_3d(sphere_x, sphere_y, sphere_z, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, N0);

		// Build a +x-axis
		G_rgb(1, 0.2, 0.2);
		double M1[4][4];
		double N1[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		M3d_make_movement_sequence_matrix(M1, _i, T_n, T_type, T_param);
		M3d_mat_mult(N1, view, M1);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N1);

		// Build a +y-axis
		G_rgb(1, 1, 1);
		double M2[4][4];
		double N2[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = -90;	T_n++;
		M3d_make_movement_sequence_matrix(M2, _i, T_n, T_type, T_param);
		M3d_mat_mult(N2, view, M2);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N2);

		// Build a +z-axis
		G_rgb(0.3, 0.2, 1);
		double M3[4][4];
		double N3[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		M3d_make_movement_sequence_matrix(M3, _i, T_n, T_type, T_param);
		M3d_mat_mult(N3, view, M3);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N3);

		// Build a x-y link
		double half_length = 2 * sqrt(2);
		G_rgb(0, 0.5, 0.5);
		double M4[4][4];
		double N4[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = -45;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M4, _i, T_n, T_type, T_param);
		M3d_mat_mult(N4, view, M4);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N4);

		// Build a x-z link
		G_rgb(0, 0.5, 0.5);
		double M5[4][4];
		double N5[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -45;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M5, _i, T_n, T_type, T_param);
		M3d_mat_mult(N5, view, M5);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N5);

		// Build a y-z link
		G_rgb(0, 0.5, 0.5);
		double M6[4][4];
		double N6[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M6, _i, T_n, T_type, T_param);
		M3d_mat_mult(N6, view, M6);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N6);

		if (G_no_wait_key() == 'q' || frame >= 100)
			return 0;

		char file_name[100];
		G_display_image();
		sprintf(file_name, "%s/frame_%04d.xwd", OUTPUT_PATH, frame);
		G_save_image_to_file(file_name);

		frame++;
	}

	return 0;
}
