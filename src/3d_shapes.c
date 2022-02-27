#include <sys/stat.h>
#include <sys/types.h>
#include <shape_3d.h>
#include <shape_color.h>
#include <g3d.h>
#include <lg.h>

#define OUTPUT_PATH "out/Graph_3D"

void initialize() {
	initialize_texture_maps();
	initialize_z_buffer();
	LG_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create frame directory
	mkdir(OUTPUT_PATH, 0777);
}

void save_image(int frame) {
	char file_name[100];
	sprintf(file_name, "%s/frame_%04d.xwd", OUTPUT_PATH, frame);
	LG_display_image();
	LG_save_image_to_file(file_name);
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
		graph_3d(sphere_xyz, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, N0, origin_rgb);

		// Build a +x-axis
		double M1[4][4];
		double N1[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		M3d_make_movement_sequence_matrix(M1, _i, T_n, T_type, T_param);
		M3d_mat_mult(N1, view, M1);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, 0, 4, N1, x_rgb);

		// Build a +y-axis
		double M2[4][4];
		double N2[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = -90;	T_n++;
		M3d_make_movement_sequence_matrix(M2, _i, T_n, T_type, T_param);
		M3d_mat_mult(N2, view, M2);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, 0, 4, N2, y_rgb);

		// Build a +z-axis
		double M3[4][4];
		double N3[4][4];
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		M3d_make_movement_sequence_matrix(M3, _i, T_n, T_type, T_param);
		M3d_mat_mult(N3, view, M3);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, 0, 4, N3, z_rgb);

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
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -half_length, half_length , N4, diagonal_rgb);

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
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -half_length, half_length , N5, diagonal_rgb);

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
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -half_length, half_length , N6, diagonal_rgb);

		if (LG_no_wait_key() == 'q' || frame >= 100)
			return 0;

		save_image(frame);

		frame++;
	}

	return 0;
}
