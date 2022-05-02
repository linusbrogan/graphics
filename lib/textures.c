#include <math.h>
#include <stdio.h>
#include <graphics.h>
#include <xwd.h>

enum texture_map {
	TM_SOLID_COLOR = -1,
	TM_CHECKERBOARD,
	TM_CLOCK,
	TM_MANDELBROT_SET,
	TM_EARTH,
	TM_WOOD,
	TM_COUNT
};

char *TEXTURE_MAP_FILES[TM_COUNT] = {
	"asset/checkerboard.xwd",
	"asset/clock.xwd",
	"asset/mandelbrot_set.xwd",
	"asset/earth.xwd",
	"asset/wood.xwd"
};

int TEXTURE_IDS[TM_COUNT];

void initialize_texture_maps() {
	for (int i = 0; i < TM_COUNT; i++) {
		TEXTURE_IDS[i] = init_xwd_map_from_file(TEXTURE_MAP_FILES[i]);
		if (TEXTURE_IDS[i] == -1) {
			fprintf(stderr, "Error: Failed to load texture %d\n", i);
		}
	}
}

int min(int a, int b) {
	if (a < b) return a;
	return b;
}

int max(int a, int b) {
	if (a > b) return a;
	return b;
}

void texture_map(enum texture_map tm, double u, double v, double *rgb) {
	if (tm == TM_SOLID_COLOR) return;

	// Get XWD dimensions
	int d[2];
	int error = get_xwd_map_dimensions(TEXTURE_IDS[tm], d);
	if (error == -1) {
		fprintf(stderr, "Error accessing texture %d with XWD ID %d\n", tm, TEXTURE_IDS[tm]);
		return;
	}

	// Map point into XWD
	int x = floor(u * d[_X]);
	x = max(0, min(x, d[_X] - 1));
	int y = floor(v * d[_Y]);
	y = max(0, min(y, d[_Y] - 1));

	get_xwd_map_color(TEXTURE_IDS[tm], x, y, rgb);
}
