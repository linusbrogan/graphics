#include "FPToolkit.c"

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc < 3) {
		printf("usage: pgm_name movie_prefix num_frames\n");
		exit(0);
	}

	// Initialize graphics
	G_init_graphics(1000, 1000);

	// Contruct file names and render frames
	int num_frames = atoi(argv[2]);
	char file_name[100];
	for (int i = 0; i < num_frames; i++) {
		sprintf(file_name, "%s%04d.xwd", argv[1], i);
		G_get_image_from_file(file_name, 0, 0) ;
		G_wait_key();
	}
}
