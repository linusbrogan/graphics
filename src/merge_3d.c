#include <stdio.h>
#include <unistd.h>
#include <graphics.h>
#include <xwd.h>

enum merge_3d_args {
	PROGRAM,
	PREFIX,
	FRAME_START,
	FRAME_END,
	ARGC
};

void merge_color(double left[3], double right[3], double merged[3]) {
	merged[_R] = left[_R];
	merged[_G] = right[_G];
	merged[_B] = right[_B];
}

void check_xwd_error(int error, char *message) {
	if (error == -1) {
		fprintf(stderr, "%s\n", message);
		exit(-1);
	}
}

void merge(int left, int right, int merged) {
	int d[2];
	int error = get_xwd_map_dimensions(merged, d);
	check_xwd_error(error, "Could not get (left) map dimensions");
	for (int x = 0; x < d[_X]; x++) {
		for (int y = 0; y < d[_Y]; y++) {
			double left_rgb[3];
			error = get_xwd_map_color(left, x, y, left_rgb);
			check_xwd_error(error, "Could not get left map color");
			double right_rgb[3];
			error = get_xwd_map_color(right, x, y, right_rgb);
			check_xwd_error(error, "Could not get right map color");
			double rgb[3];
			merge_color(left_rgb, right_rgb, rgb);
			error = set_xwd_map_color(merged, x, y, rgb[_X], rgb[_Y], rgb[_B]);
			check_xwd_error(error, "Could not set merged map color");
		}
	}
}

void proceed(int right_id, int frame, char *argv[]) {
	if (right_id < MAXNUMXWDMAPS - 5) return;
	char current_frame[10];
	sprintf(current_frame, "%d", frame);
	char *argv2[ARGC + 1];
	for (int i = 0; i < ARGC; i++) {
		argv2[i] = argv[i];
	}
	argv2[FRAME_START] = current_frame;
	argv2[ARGC] = NULL;
	execvp(argv2[PROGRAM], argv2);
}

int main(int argc, char *argv[]) {
	// Parse arguments
	if (argc < ARGC) {
		printf("Usage: merge_3d prefix frame_start frame_end\n");
		exit(1);
	}
	char *prefix = argv[PREFIX];
	int frame_start = atoi(argv[FRAME_START]);
	int frame_end = atoi(argv[FRAME_END]);

	int render_frame_id = -1;
	for (int frame = frame_start; frame <= frame_end; frame++) {
		// Load frames
		char file_name[100];
		sprintf(file_name, "%sleft_%04d.xwd", prefix, frame);
		int left_id = init_xwd_map_from_file(file_name);
		check_xwd_error(left_id, "Could not load left map");
		sprintf(file_name, "%sright_%04d.xwd", prefix, frame);
		int right_id = init_xwd_map_from_file(file_name);
		check_xwd_error(left_id, "Could not load right map");

		proceed(right_id, frame, argv);

		// Create merge frame
		if (render_frame_id == -1) {
			int d[2];
			int error = get_xwd_map_dimensions(left_id, d);
			check_xwd_error(error, "Could not get map dimensions");
			render_frame_id = create_new_xwd_map(d[_X], d[_Y]);
			check_xwd_error(render_frame_id, "Could not create merged map");
		}

		// Merge frames
		merge(left_id, right_id, render_frame_id);
		sprintf(file_name, "%s3d_%04d.xwd", prefix, frame);
		xwd_map_to_named_xwd_file(render_frame_id, file_name);
	}

	return 0;
}
