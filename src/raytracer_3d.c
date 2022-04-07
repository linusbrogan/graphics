#include <sys/stat.h>
#include <sys/types.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 100

void initialize() {
	LG_init_graphics(1080, 720);
	initialize_texture_maps();

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
		t = 2 * M_PI * frame / FRAMES;
		double eye[3] = {0, 0, 0};
		double coi[3] = {sin(-t), 0, cos(t)};
		double up[3] = {eye[_X], eye[_Y], eye[_Z] + 1};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);

		// Set light
		//double light[3] = {half_width - 1 + sin(1.2 * t), cos(t), 1.5 * (half_height + sin(t))};
		//M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Build the planet
		double planet_radius = 10000;
		object_type[objects] = OBJ_SPHERE;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = planet_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 57;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = -38;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 12000;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 6500;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 12000;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the space station
		double half_height = 0.5;
		// Make the space station transformation sequence
		double S[4][4];
		double S_i[4][4];
		T_n = 0;
		T_type[T_n] = RX;	T_param[T_n] = 15;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(S, S_i, T_n, T_type, T_param);
		M3d_mat_mult(S, view, S);
		M3d_mat_mult(S_i, S_i, view_i);

		// Build the upper ring
		object_type[objects] = OBJ_SS_RING;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the lower ring
		object_type[objects] = OBJ_SS_RING;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		double beam_radius = 0.1;
		// Build the upper cross-bar 1
		object_type[objects] = OBJ_CYLINDER;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the upper cross-bar 2
		object_type[objects] = OBJ_CYLINDER;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the lower cross-bar 1
		object_type[objects] = OBJ_CYLINDER;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the lower cross-bar 2
		object_type[objects] = OBJ_CYLINDER;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build the central axis
		object_type[objects] = OBJ_CYLINDER;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_EARTH; // blue
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.7;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		M3d_mat_mult(object_matrix[objects], S, M);
		M3d_mat_mult(M_i, M_i, S);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;


		render();
		save_image(frame);
		frame++;
	}

	return 0;
}
