#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 200

void initialize() {
	LG_init_graphics(1080, 720);
	initialize_texture_maps();
	HALF_ANGLE = M_PI / 4;

	// Create frame directory
	mkdir(OUTPUT_PATH, 0777);
}

void save_image(int frame, char *prefix) {
	char file_name[100];
	sprintf(file_name, "%s/%s_%04d.xwd", OUTPUT_PATH, prefix, frame);
	LG_display_image();
	LG_save_image_to_file(file_name);
}

void apply_transformation(int object_start, int object_end, double T[4][4], double T_i[4][4]) {
	for (int i = object_start; i < object_end; i++) {
		M3d_mat_mult(object_matrix[i], T, object_matrix[i]);
		M3d_mat_mult(object_matrix_i[i], object_matrix_i[i], T_i);
	}
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

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double M[4][4];
	double M_i[4][4];

	int frame = frame_start;
	int side = -1;
	while (1) {
		if (LG_no_wait_key() == 'q' || frame >= frame_stop)
			return 0;

		printf("Rendering frame %04d\n", frame);

		// Reset frame
		LG_rgb(0, 0, 0);
		LG_clear();
		clear_objects();

		// Configure frame
		double t = frame * TAU / FRAMES;
		double eye[3] = {5 * cos(t * 2), 5.2 * sin(t * 2), cos(t)};
		double coi[3] = {0, 0, 0};
		double up[3] = {eye[_X], eye[_Y], eye[_Z] + 1};
		double eye_spacing = 0.05;

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view_3d(view, view_i, eye, coi, up, eye_spacing * side);

		// Set light
		double light[3] = {-10, 10, 10};
		M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Build columns
		for (int i = 0; i < 9; i++) {
			object_type[objects] = OBJ_CYLINDER;
			object_texture[objects] = TM_SOLID_COLOR;
			object_color[objects][_R] = 1;
			object_color[objects][_G] = 1;
			object_color[objects][_B] = 1;
			object_reflectivity[objects] = 0.2;
			object_opacity[objects] = 1;
			T_n = 0;
			T_type[T_n] = SX;	T_param[T_n] = 0.1;	T_n++;
			T_type[T_n] = SY;	T_param[T_n] = 0.1;	T_n++;
			T_type[T_n] = SZ;	T_param[T_n] = 0.4;	T_n++;
			T_type[T_n] = TX;	T_param[T_n] = i % 3;	T_n++;
			T_type[T_n] = TY;	T_param[T_n] = i / 3;	T_n++;
			M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
			M3d_mat_mult(object_matrix[objects], view, M);
			M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
			objects++;
		}

		render();

	if (side < 0) {
			save_image(frame, "left");
			side = 1;
		} else {
			save_image(frame, "right");
			side = -1;
			frame++;
		}
	}

	return 0;
}
