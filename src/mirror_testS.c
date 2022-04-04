#include <sys/stat.h>
#include <sys/types.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Mirror_Test"
#define FRAMES 100

void initialize() {
	initialize_texture_maps();
	LG_init_graphics(800, 800);
	HALF_ANGLE = M_PI / 3;

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
		double eye[3] = {
			25 * cos(M_PI + t),
			25 * sin(M_PI + t) + 25,
			-10
		};
		double coi[3] = {0, 0, 50};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);

		// Set light
		double light[3] = {0, 20, 30};
		M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Build objects
		object_type[objects] = OBJ_HYPERBOLOID;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 55;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		object_type[objects] = OBJ_HYPERBOLOID;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 25;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		object_type[objects] = OBJ_HYPERBOLOID;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -25;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		object_type[objects] = OBJ_HYPERBOLOID;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Chandelier
		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 0.7;
		object_color[objects][_G] = 0.7;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 25;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 0.7;
		object_color[objects][_G] = 0.7;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 2;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 25;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Mirrors
		object_type[objects] = OBJ_PLANE;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0;
		object_reflectivity[objects] = 0.8;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 89;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 30;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = -10.2;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		object_type[objects] = OBJ_PLANE;
		object_color[objects][_R] = 0;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 1;
		object_reflectivity[objects] = 0.8;
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 20;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 150;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 55;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 20;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// World sphere
		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 0.2;
		object_color[objects][_G] = 0.2;
		object_color[objects][_B] = 0.2;
		object_reflectivity[objects] = -1; // No light model
		object_opacity[objects] = 1;
		object_texture[objects] = TM_SOLID_COLOR;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 100;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 100;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 100;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 50;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = 0;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		//objects++;

		render();
		save_image(frame);
		frame++;
	}

	return 0;
}
