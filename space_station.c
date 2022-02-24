#include <sys/stat.h>
#include <sys/types.h>
#include "light_model.c"
#include "2d_shape_functions.c"
#include "3d_shape_functions.c"
#include "XWD_TOOLS_03/xwd_tools_03.c"
#include "shape_colors.c"

#define NUM_PTS 1000
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define WINDOW_SIZE (MAX(WINDOW_WIDTH, WINDOW_HEIGHT))
#define DX (-((WINDOW_SIZE) - (WINDOW_WIDTH)) / 2)
#define DY (-((WINDOW_SIZE) - (WINDOW_HEIGHT)) / 2)
#define HALF_ANGLE (M_PI / 6)
#define H (tan(HALF_ANGLE))
#define OUTPUT_PATH "Space_Station"
#define HITHER (1e-3)
#define YON (1e50)

double Z_BUFFER[WINDOW_SIZE][WINDOW_SIZE];

void space_station_color(double u, double v, double *rgb) {
	rgb[0] = 0.5;
	rgb[1] = 0.5;
	rgb[2] = 0.5;
}

double torus_x(double u, double v) {
	double R = 1;
	double r = 0.1;
	return (R + r * cos(v)) * cos(u);
}
double torus_y(double u, double v) {
	double R = 1;
	double r = 0.1;
	return (R + r * cos(v)) * sin(u);
}
double torus_z(double u, double v) {
	double R = 1;
	double r = 0.1;
	double sign = v >= 2 * M_PI ? 1 : -1;
	double height = 1 * sign / 2;
	return height + r * sin(v);
}

double cyl_x(double u, double v) {
	return u;
}

double cyl_y(double u, double v) {
	return 0.1 * cos(v);
}

double cyl_z(double u, double v) {
	double sign = v >= 2 * M_PI ? 1 : -1;
	double height = 1 * sign / 2;
	return height + 0.1 * sin(v);
}

double cap_x(double u, double v) {
	double r = v;
	return r * cos(u);
}

double cap_y(double u, double v) {
	double r = v;
	return r * sin(u);
}

double cap_z(double u, double v) {
	return sgn(v);
}

void initialize_z_buffer() {
	for (int x = 0; x < WINDOW_SIZE; x++) {
		for (int y = 0; y < WINDOW_SIZE; y++) {
			Z_BUFFER[x][y] = YON;
		}
	}
}

int project(double x, double z) {
	double x_bar = x / z;
	double x_prime = (x_bar / H + 1) * WINDOW_SIZE / 2;
	return round(x_prime);
}

void set_color(
	double (*f_x)(double, double),
	double (*f_y)(double, double),
	double (*f_z)(double, double),
	double u,
	double du,
	double v,
	double dv,
	double T[4][4],
	void (*inherent_rgb)(double, double, double *)
) {
	double P[3] = {
		f_x(u, v),
		f_y(u, v),
		f_z(u, v)
	};
	double u2 = u + du;
	double Q[3] = {
		f_x(u2, v),
		f_y(u2, v),
		f_z(u2, v)
	};
	double v2 = v + dv;
	double R[3] = {
		f_x(u, v2),
		f_y(u, v2),
		f_z(u, v2)
	};

	M3d_mat_mult_pt(P, T, P);
	M3d_mat_mult_pt(Q, T, Q);
	M3d_mat_mult_pt(R, T, R);

	double A[3];
	double B[3];
	for (int i = 0; i < 3; i++) {
		A[i] = Q[i] - P[i];
		B[i] = R[i] - P[i];
	}

	double n[3];
	M3d_x_product(n, A, B);

	double eye[3] = {0, 0, 0};
	double irgb[3];
	inherent_rgb(u, v, irgb);
	double rgb[3];
	Light_Model(irgb, eye, P, n, rgb);
	G_rgb(rgb[0], rgb[1], rgb[2]);
}

void graph_3d(
	double (*f_x)(double, double),
	double (*f_y)(double, double),
	double (*f_z)(double, double),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double T[4][4],
	void (*rgb)(double, double, double *)
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
				if (z > HITHER && z < Z_BUFFER[x][y]) {
					Z_BUFFER[x][y] = z;
					set_color(f_x, f_y, f_z, u, du, v, dv, T, rgb);
					G_point(x + DX, y + DY);
				}
			}
		}
	}
}

int main() {
	initialize_texture_maps();

	// Initialize screen
	G_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);

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
		initialize_z_buffer();
		G_rgb(0, 0, 0);
		G_clear();

		// Configure frame
		t = 0.01 * frame;
		double eye[3] = {0, 0, 0};
		double coi[3] = {sin(t), 0, cos(t)};
		double up[3] = {eye[0], eye[1] + 1, eye[2]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);


		double _i[4][4];

		// Build the stars
		double M0X[4][4];
		double N0X[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 10;	T_n++;
		M3d_make_movement_sequence_matrix(M0X, _i, T_n, T_type, T_param);
		M3d_mat_mult(N0X, view, M0X);
		//graph_3d(sphere_x, sphere_y, sphere_z, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, N0X, star_map);


		// Build the planet
		double M0[4][4];
		double N0[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 6000;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 10000;	T_n++;
		M3d_make_movement_sequence_matrix(M0, _i, T_n, T_type, T_param);
		M3d_mat_mult(N0, view, M0);
		graph_3d(sphere_x, sphere_y, sphere_z, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, N0, clock_map);

		// Build the torus pair
		double M1[4][4];
		double N1[4][4];
		T_n = 0;
		T_type[T_n] = RX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M1, _i, T_n, T_type, T_param);
		M3d_mat_mult(N1, view, M1);
		graph_3d(snorus_x, snorus_y, snorus_z, 0, 2 * M_PI, 0, 4 * M_PI, N1, space_station_color);

		// Build one cross-axis pair
		double M2[4][4];
		double N2[4][4];
		T_n = 0;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M2, _i, T_n, T_type, T_param);
		M3d_make_movement_sequence_matrix(M2, _i, T_n, T_type, T_param);
		M3d_mat_mult(N2, view, M2);
		graph_3d(cyl_x, cyl_y, cyl_z, -1, 1, 0, 4 * M_PI, N2, space_station_color);

		// Build another cross-axis pair
		double M3[4][4];
		double N3[4][4];
		T_n = 0;
		T_type[T_n] = RZ;	T_param[T_n] = -45;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M3, _i, T_n, T_type, T_param);
		M3d_make_movement_sequence_matrix(M3, _i, T_n, T_type, T_param);
		M3d_mat_mult(N3, view, M3);
		graph_3d(cyl_x, cyl_y, cyl_z, -1, 1, 0, 4 * M_PI, N3, space_station_color);

		// Build the center connection
		double M4[4][4];
		double N4[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M4, _i, T_n, T_type, T_param);
		M3d_make_movement_sequence_matrix(M4, _i, T_n, T_type, T_param);
		M3d_mat_mult(N4, view, M4);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -0.7, 0.7, N4, space_station_color);

		// Build the center connection end cap
		double M5[4][4];
		double N5[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.2;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.2;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.7;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M5, _i, T_n, T_type, T_param);
		M3d_make_movement_sequence_matrix(M5, _i, T_n, T_type, T_param);
		M3d_mat_mult(N5, view, M5);
		graph_3d(cap_x, cap_y, cap_z, 0, 2 * M_PI, -1, 1, N5, space_station_color);


		if (G_no_wait_key() == 'q' || frame >= 1000)
			return 0;

		char file_name[100];
		G_display_image();
		sprintf(file_name, "%s/frame_%04d.xwd", OUTPUT_PATH, frame);
		G_save_image_to_file(file_name);

		frame++;
	}

	return 0;
}
