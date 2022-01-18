#include "FPToolkit.c"

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc < 4) {
		printf("usage: pgm_name width height frame1 [frame2 [...]]\n");
		exit(0);
	}

	// Initialize graphics
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	G_init_graphics(width, height);

	// Display frames
	for (int i = 3; i < argc; i++) {
		G_get_image_from_file(argv[i], 0, 0) ;
		G_wait_key();
	}
}
