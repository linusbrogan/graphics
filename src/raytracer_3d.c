#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 100

void initialize() {
	srand48(&initialize);
	LG_init_graphics(1080 / 3, 720 / 3);
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

	// Make bubbles
	int num_bubbles = 50;
	double bubbles[3 * num_bubbles];
	for (int i = 0; i < 3 * num_bubbles; i++) {
		bubbles[i] = drand48();
	}

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
		double eye[3] = {0, 0, 0.5};
		double coi[3] = {cos(t), sin(t), cos(2 * t)};
		double up[3] = {eye[_X], eye[_Y], eye[_Z] + 1};
		double eye_spacing = 0.02;

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view_3d(view, view_i, eye, coi, up, eye_spacing * side);

		// Set light
		double light[3] = {3.5, 1.5, 2.5};
		M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Parameters
		double glass_inner_radius = 1;
		double glass_thickness = 0.05;
		double glass_height = 3;
		double water_height = glass_height * 0.6;
		double water_radius = glass_inner_radius * 0.99;
		double n_glass = 1.5;
		double n_water = 1.33;

		// Build bubbles
		double bubble_radius = 0.1;
		int N = 5;
		for (int i = -N; i < N; i++) {
			for (int j = -N; j < N; j++) {
				for (int k = -N; k < N; k++) {
					if ((i + j + k + 3 * N) % 2 == 0) continue;
					object_type[objects] = OBJ_SPHERE;
					object_texture[objects] = TM_SOLID_COLOR;
					object_color[objects][_R] = ((double) (i + N) / (2 * N) + 1) / 2;
					object_color[objects][_G] = ((double) (j + N) / (2 * N) + 1) / 2;
					object_color[objects][_B] = ((double) (k + N) / (2 * N) + 1) / 2;
					object_reflectivity[objects] = 0.2;
					object_refractive_index[objects] = 1;
					object_opacity[objects] = 0.4;
					T_n = 0;
					T_type[T_n] = SX;	T_param[T_n] = bubble_radius;	T_n++;
					T_type[T_n] = SY;	T_param[T_n] = bubble_radius;	T_n++;
					T_type[T_n] = SZ;	T_param[T_n] = bubble_radius;	T_n++;
					T_type[T_n] = TX;	T_param[T_n] = i;	T_n++;
					T_type[T_n] = TY;	T_param[T_n] = j;	T_n++;
					T_type[T_n] = TZ;	T_param[T_n] = k;	T_n++;
					M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
					objects++;
				}
			}
		}

		apply_transformation(0, objects, view, view_i);
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
