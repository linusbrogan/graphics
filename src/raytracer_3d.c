#include <sys/stat.h>
#include <sys/types.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 100

void initialize() {
	LG_init_graphics(600, 400);
	initialize_texture_maps();
	HALF_ANGLE = M_PI / 4;

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
		t = 0.1 * frame;
		double rrr = 10 * (-1 + 2 * frame / 100.0);
		double eye[3] = {5,3,8};//{0, sin(t) * rrr, rrr * cos(t)};
		double coi[3] = {0, 0, 0};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);

		double half_width = 5;
		double half_height = 8;

		// Set light
		double *light = eye;
		M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Build lower platform
		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 0;
		object_color[objects][_B] = 0;
		object_opacity[objects] = 1;
		object_reflectivity[objects] = 0;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Build lower platform
		object_type[objects] = OBJ_TORUS;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 1;
		object_color[objects][_B] = 1;
		object_opacity[objects] = 1;
		object_reflectivity[objects] = 0;
		object_texture[objects] = TM_CHECKERBOARD;
		T_n = 0;
//		T_type[T_n] = TZ;	T_param[T_n] = 0;	T_n++;
		double tss = 1.75;
		T_type[T_n] = SX;	T_param[T_n] = tss;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = tss;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = tss;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = TAU * frame / 10.0;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 2 * TAU * frame / 10.0;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 3 * TAU * frame / 10.0;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 3;	T_n++;
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
