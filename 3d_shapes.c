#include <sys/stat.h>
#include <sys/types.h>
#include "light_model.c"
#include "2d_shape_functions.c"
#include "3d_shape_functions.c"
#include "XWD_TOOLS_03/xwd_tools_03.c"
#include "shape_colors.c"

#define RESOLUTION 500
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define WS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define WINDOW_SIZE (WS_MAX(WINDOW_WIDTH, WINDOW_HEIGHT))
#define DX (-((WINDOW_SIZE) - (WINDOW_WIDTH)) / 2)
#define DY (-((WINDOW_SIZE) - (WINDOW_HEIGHT)) / 2)
#define HALF_ANGLE (M_PI / 6)
#define H (tan(HALF_ANGLE))
#define OUTPUT_PATH "Graph_3D"
#define HITHER (1e-3)
#define YON (1e50)
#define LG_USE_FPT

enum coordinate {
	_X = 0,
	_Y,
	_Z
};

enum color {
	_R = 0,
	_G,
	_B
};

double Z_BUFFER[WINDOW_WIDTH][WINDOW_HEIGHT];

// Graphics shims
#ifndef LG_USE_FPT
int LG_WINDOW_ID = 0;
double LG_RGB[3] = {0, 0, 0};
#endif
void LG_init_graphics(int width, int height) {
#ifdef LG_USE_FPT
	G_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
#else
	LG_WINDOW_ID = create_new_xwd_map(width, height);
	if (LG_WINDOW_ID == -1) {
		fprintf(stderr, "Could not initialize XWD map.");
		exit(LG_WINDOW_ID);
	}
#endif
}
void LG_rgb(double r, double g, double b) {
#ifdef LG_USE_FPT
	G_rgb(r, g, b);
#else
	LG_RGB[_R] = r;
	LG_RGB[_G] = g;
	LG_RGB[_B] = b;
#endif
}
void LG_point(int x, int y) {
#ifdef LG_USE_FPT
	G_point(x, y);
#else
	double r = LG_RGB[_R];
	double g = LG_RGB[_G];
	double b = LG_RGB[_B];
	set_xwd_map_color(LG_WINDOW_ID, x, y, r, g, b);
#endif
}
void LG_clear() {
#ifdef LG_USE_FPT
	G_clear();
#else
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_WIDTH; y++) {
			LG_point(x, y);
		}
	}
#endif
}
int LG_no_wait_key() {
#ifdef LG_USE_FPT
	return G_no_wait_key();
#else
	return -1;
#endif
}
void LG_display_image() {
#ifdef LG_USE_FPT
	G_display_image();
#endif
}
void LG_save_image_to_file(char *file_name) {
#ifdef LG_USE_FPT
	G_save_image_to_file(file_name);
#else
	xwd_map_to_named_xwd_file(LG_WINDOW_ID, file_name) ;
#endif
}



void initialize_z_buffer() {
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			Z_BUFFER[x][y] = YON;
		}
	}
}

void initialize() {
	initialize_texture_maps();
	initialize_z_buffer();
	LG_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create frame directory
	mkdir(OUTPUT_PATH, 0777);
}

int project(double w, double z, enum coordinate sign) {
	double w_bar = w / z;
	double w_prime = (w_bar / H + 1) * WINDOW_SIZE / 2;
	int w_px = round(w_prime);
	if (sign == _X) w_px += DX;
	if (sign == _Y) w_px += DY;
	return w_px;
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
	LG_rgb(rgb[0], rgb[1], rgb[2]);
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
	double du = (u_end - u_start) / RESOLUTION;
	double dv = (v_end - v_start) / RESOLUTION;
	for (double u = u_start; u <= u_end; u += du) {
		for (double v = v_start; v <= v_end; v += dv) {
			double point[3] = {
				f_x(u, v),
				f_y(u, v),
				f_z(u, v)
			};
			M3d_mat_mult_pt(point, T, point);
			double z = point[2];
			int x = project(point[0], z, _X);
			int y = project(point[1], z, _Y);
			if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
				if (z > HITHER && z < Z_BUFFER[x][y]) {
					Z_BUFFER[x][y] = z;
					set_color(f_x, f_y, f_z, u, du, v, dv, T, rgb);
					LG_point(x, y);
				}
			}
		}
	}
}

int main() {
	initialize();

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];

	int frame = 0;
	double t = 0;
	while (1) {
		// Reset frame
		initialize_z_buffer();
		LG_rgb(0, 0, 0);
		LG_clear();

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

		// Build an origin point
		double M0[4][4];
		double N0[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.25;	T_n++;
		M3d_make_movement_sequence_matrix(M0, _i, T_n, T_type, T_param);
		M3d_mat_mult(N0, view, M0);
		graph_3d(sphere_x, sphere_y, sphere_z, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, N0, origin_rgb);

		// Build a +x-axis
		double M1[4][4];
		double N1[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		M3d_make_movement_sequence_matrix(M1, _i, T_n, T_type, T_param);
		M3d_mat_mult(N1, view, M1);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N1, x_rgb);

		// Build a +y-axis
		double M2[4][4];
		double N2[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = -90;	T_n++;
		M3d_make_movement_sequence_matrix(M2, _i, T_n, T_type, T_param);
		M3d_mat_mult(N2, view, M2);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N2, y_rgb);

		// Build a +z-axis
		double M3[4][4];
		double N3[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		M3d_make_movement_sequence_matrix(M3, _i, T_n, T_type, T_param);
		M3d_mat_mult(N3, view, M3);
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, 0, 4, N3, z_rgb);

		// Build a x-y link
		double half_length = 2 * sqrt(2);
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
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N4, diagonal_rgb);

		// Build a x-z link
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
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N5, diagonal_rgb);

		// Build a y-z link
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
		graph_3d(cylinder_x, cylinder_y, cylinder_z, 0, 2 * M_PI, -half_length, half_length , N6, diagonal_rgb);

		if (LG_no_wait_key() == 'q' || frame >= 100)
			return 0;

		char file_name[100];
		LG_display_image();
		sprintf(file_name, "%s/frame_%04d.xwd", OUTPUT_PATH, frame);
		LG_save_image_to_file(file_name);

		frame++;
	}

	return 0;
}
