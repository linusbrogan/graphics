#include <sys/stat.h>
#include <sys/types.h>
#include <shape_3d.h>
#include <shape_color.h>
#include <g3d.h>
#include <lg.h>

#define OUTPUT_PATH "out/Space_Station"
#define FRAMES 100

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
	double M[4][4];
	double _i[4][4];

	int frame = 0;
	double t = 0;
	while (1) {
		printf("Rendering frame %04d\n", frame);

		// Reset frame
		initialize_z_buffer();
		LG_rgb(0, 0, 0);
		LG_clear();

		// Configure frame
		t = 2 * M_PI * frame / FRAMES;
		double eye[3] = {0, 0, 0};
		double coi[3] = {sin(-t), 0, cos(t)};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view(view, view_i, eye, coi, up);

		// Build a demo sphere
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.25;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 2;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		graph_3d(sphere_xyz, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, M, unit_checkerboard_map);

		if (LG_no_wait_key() == 'q' || frame >= FRAMES)
			return 0;

		save_image(frame);
		frame++;
	}

	return 0;
}
