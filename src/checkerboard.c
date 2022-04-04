#include <lg.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: checkerboard height [width]\n");
		return 0;
	}

	int width = atoi(argv[1]);
	int height = width;
	if (argc > 2) height = atoi(argv[2]);

	LG_init_graphics(width, height);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int scale = 1 + ((x + y) % 2);
			double c = scale / 3.0;
			LG_rgb(c, c, c);
			LG_point(x, y);
		}
	}

	LG_display_image();

	char file_name[100];
	sprintf(file_name, "asset/checkerboard_%dx%d.xwd", width, height);
	LG_save_image_to_file(file_name);

	return 0;
}
