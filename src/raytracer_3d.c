#include <sys/stat.h>
#include <sys/types.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 100

void initialize() {
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

int main(int argc, char *argv[]) {
	int frame_start = 0;
	int frame_stop = FRAMES;
	if (argc >= 2) {
		frame_start = atoi(argv[1]);
	}
	if (argc >= 3) {
		frame_stop = atoi(argv[2]);
	}

	initialize();

	// Set light
	light_in_eye_space[_X] = -5;
	light_in_eye_space[_Y] = 10;
	light_in_eye_space[_Z] = 2;

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double M[4][4];
	double M_i[4][4];

	int frame = frame_start;
	double t = 0;
	while (1) {
		if (LG_no_wait_key() == 'q' || frame >= frame_stop)
			return 0;

		printf("Rendering frame %04d\n", frame);

		// Reset frame
		LG_rgb(0, 0, 0);
		LG_clear();
		objects = 0;

		// Configure frame
		t = 0.01 * frame;
		double eye[3] = {15 * cos(2 * M_PI * t), 6 * t, 7 * sin(2 * M_PI * t)};
		double coi[3] = {1, 2, 0.5};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);

		// Build an origin point
		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.25;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a +x-axis
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 0.2;
		object_color[objects][_B] = 0.2;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a +y-axis
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 1;
		object_color[objects][_B] = 1;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = -90;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a +z-axis
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 0.3;
		object_color[objects][_G] = 0.2;
		object_color[objects][_B] = 1;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a x-y link
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.5;
		object_color[objects][_B] = 0.5;
		object_reflectivity[objects] = 0.2;
		double diagonal_length = 2 * sqrt(2);
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = diagonal_length;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = -45;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a x-z link
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.5;
		object_color[objects][_B] = 0.5;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = diagonal_length;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -45;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build a y-z link
		object_type[objects] = OBJ_CYLINDER;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.5;
		object_color[objects][_B] = 0.5;
		object_reflectivity[objects] = 0.2;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = diagonal_length;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		render();
		save_image(frame);
		frame++;
	}

	return 0;
}
