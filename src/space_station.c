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
	double _i[4][4];

	int frame = frame_start;
	double t = 0;
	while (1) {
		if (LG_no_wait_key() == 'q' || frame >= frame_stop)
			return 0;

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

		// Build the planet
		double planet_radius = 10000;
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
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, view, M);
		graph_3d(sphere_xyz, 0, 2 * M_PI, -M_PI / 2, M_PI / 2, M, earth_map);

		// Build the space station
		double half_height = 0.5;
		// Make the space station transformation sequence
		double S[4][4];
		T_n = 0;
		T_type[T_n] = RX;	T_param[T_n] = 15;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = -35;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 5;	T_n++;
		M3d_make_movement_sequence_matrix(S, _i, T_n, T_type, T_param);
		M3d_mat_mult(S, view, S);

		// Build the upper ring
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_ring_xyz, 0, 2 * M_PI, 0, 2 * M_PI, M, space_grey);

		// Build the lower ring
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_ring_xyz, 0, 2 * M_PI, 0, 2 * M_PI, M, space_grey);

		double beam_radius = 0.1;
		// Build the upper cross-bar 1
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the upper cross-bar 2
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the lower cross-bar 1
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the lower cross-bar 2
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = beam_radius;	T_n++;
		T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
		T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = -half_height;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(cylinder_xyz, 0, 2 * M_PI, -1, 1, M, space_grey);

		// Build the central axis
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 0.15;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.15;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(space_station_central_axis_xyz, 0, 2 * M_PI, -0.7, 0.7, M, space_grey);

		// Build the central axis upper cap
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(disk_xyz, 0, 0.2, 0, 2 * M_PI, M, space_grey);

		// Build the central axis lower cap
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = -0.7 ;	T_n++;
		M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
		M3d_mat_mult(M, S, M);
		graph_3d(disk_xyz, 0, 0.3, 0, 2 * M_PI, M, space_grey);

		save_image(frame);
		frame++;
	}

	return 0;
}
