#include <sys/stat.h>
#include <sys/types.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 400

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
	double cap_parameters[2] = {0, 0.3};
	double ring_parameters[2] = {1, 0.1};
	while (1) {
		if (LG_no_wait_key() == 'q' || frame >= frame_stop)
			return 0;

		printf("Rendering frame %04d\n", frame);

		// Reset frame
		LG_rgb(0, 0, 0);
		LG_clear();
		clear_objects();

		// Skip computations for empty frames
		if (frame >= 85 && frame < 255)
			goto save_images;

		// Configure frame
		double t = frame * TAU / FRAMES;
		double eye[3] = {0, 0, 0};
		double coi[3] = {sin(-t), 0, cos(t)};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};
		double eye_spacing = 0.4;

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view_3d(view, view_i, eye, coi, up, eye_spacing * side);

		// Build the planet
		object_type[objects] = OBJ_SPHERE;
		object_texture[objects] = TM_EARTH;
		double planet_radius = 10000;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = t * 3 / DEGREES;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 57;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = -38;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 12000;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 6500;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 12000;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the space station
		double half_height = 0.5;
		// Make the space station transformation sequence
		double S[4][4];
		double S_i[4][4];
		T_n = 0;
		T_type[T_n] = RZ;	T_param[T_n] = -5 * t / DEGREES;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 15;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(S, S_i, T_n, T_type, T_param);

		// Build the upper ring
		object_type[objects] = OBJ_TORUS;
		object_parameters[objects] = ring_parameters;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the lower ring
		object_type[objects] = OBJ_TORUS;
		object_parameters[objects] = ring_parameters;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		double beam_radius = 0.1;
		// Build the upper cross-bar 1
		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the upper cross-bar 2
		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the lower cross-bar 1
		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the lower cross-bar 2
		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the central axis
		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.7;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.3 / 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 0.4;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		object_type[objects] = OBJ_CYLINDER;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.3 / 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -0.4;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		object_type[objects] = OBJ_CONE;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.4;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		object_type[objects] = OBJ_CONE;
		T_n = 0;
		T_type[T_n] = RX;	T_param[T_n] = 180;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.4;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build the central axis upper cap
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = cap_parameters;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;
		//graph_3d(disk_xyz, 0, 0.2, 0, 2 * M_PI, M, space_grey);

		// Build the central axis lower cap
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = cap_parameters;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;
		//graph_3d(disk_xyz, 0, 0.3, 0, 2 * M_PI, M, space_grey);

		apply_transformation(1, objects, S, S_i);
		apply_transformation(0, objects, view, view_i);
		for (int i = 0; i < objects; i++) {
			object_color[i][_R] = 0.5;
			object_color[i][_G] = 0.5;
			object_color[i][_B] = 0.75;
		}

		render();

		save_images:
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
