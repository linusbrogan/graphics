#include "FPToolkit.c"

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc < 7) {
		printf("usage: pgm window_width window_height prefix_name start_num end_num microsecond(40000?)\n");
		exit(0);
	}

	// Parse arguments
	int window_width = atoi(argv[1]);
	int window_height = atoi(argv[2]);
	char *prefix_name = argv[3];
	int start_num = atoi(argv[4]);
	int end_num = atoi(argv[5]);
	int microseconds = atoi(argv[6]);

	// Initialize graphics
	G_init_graphics(window_width, window_height);

	// Contruct file names and render frames
	char file_name[100];
	while (1) {
		for (int i = start_num; i <= end_num; i++) {
			sprintf(file_name, "%s%04d.xwd", prefix_name, i);
			G_get_image_from_file(file_name, 0, 0) ;
			G_display_image();
			usleep(microseconds);
			char keypress = G_no_wait_key();
			if (keypress == 'q') exit(0);
		}
	}
}
