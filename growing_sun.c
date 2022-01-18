#include "FPToolkit.c"

/**
 * Draws a frame of the growing sun, centered at (x, y) with the given radius.
 * shade is how much green is in the sun.
 */
void draw_frame(double x, double y, double radius, double shade) {
	G_rgb(0, 0, 0);
	G_clear();
	G_rgb(1.0, shade, 0);
	G_fill_circle(x, y, radius);
}

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc < 5) {
		printf("pgm usage: pgm_name screen_width screen_height num_frames delta_radius\n");
		exit(0);
	}

	// Parse arguments
	int screen_width = atoi(argv[1]);
	int screen_height = atoi(argv[2]);
	int num_frames = atoi(argv[3]);
	double delta_radius = atof(argv[4]);

	// Initialize graphics
	G_init_graphics(screen_width, screen_height);

	// Specify movie details
	double x = screen_width / 2.0;
	double y = screen_height / 2.0;
	double radius = 10;
	double shade = 0;
	double delta_shade = 1.0 / num_frames;
	char file_name[100];

	//Draw frames
	for (int i = 0; i < num_frames; i++) {
		// Render
		draw_frame(x, y, radius, shade);
		G_wait_key();

		// Save image
		sprintf(file_name, "growing_sun.frame_%04d.xwd", i);
		G_save_image_to_file(file_name);

		// Update frame details
		radius += delta_radius;
		shade += delta_shade;

	}
}
