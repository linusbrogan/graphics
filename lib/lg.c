// Graphics library shims for FPT or XWD Tools
#undef LG_USE_FPT

#ifdef LG_USE_FPT
#include <fpt.h>
#endif

#include <xwd.h>
#include <graphics.h>

int LG_WINDOW_WIDTH = 0;
int LG_WINDOW_HEIGHT = 0;
#ifndef LG_USE_FPT
int LG_WINDOW_ID = -1;
int LG_INITIALIZED = 0;
double LG_RGB[3] = {0, 0, 0};
#endif

void LG_init_graphics(int width, int height) {
	LG_WINDOW_WIDTH = width;
	LG_WINDOW_HEIGHT = height;
#ifdef LG_USE_FPT
	G_init_graphics(width, height);
#else
	if (!LG_INITIALIZED) {
		LG_INITIALIZED = 1;
		LG_WINDOW_ID = create_new_xwd_map(width, height);
	}
	if (LG_WINDOW_ID == -1) {
		fprintf(stderr, "Could not initialize XWD map.");
		exit(-1);
	}

#endif
}

void LG_rgb(double r, double g, double b) {
#ifdef LG_USE_FPT
	G_rgb(r, g, b);
#else
	LG_RGB[_R] = r;
	LG_RGB[_G] = g;
	LG_RGB[_B] = b;
#endif
}

void LG_point(int x, int y) {
#ifdef LG_USE_FPT
	G_point(x, y);
#else
	double r = LG_RGB[_R];
	double g = LG_RGB[_G];
	double b = LG_RGB[_B];
	set_xwd_map_color(LG_WINDOW_ID, x, y, r, g, b);
#endif
}

void LG_clear() {
#ifdef LG_USE_FPT
	G_clear();
#else
	for (int x = 0; x < LG_WINDOW_WIDTH; x++) {
		for (int y = 0; y < LG_WINDOW_WIDTH; y++) {
			LG_point(x, y);
		}
	}
#endif
}

int LG_no_wait_key() {
#ifdef LG_USE_FPT
	return G_no_wait_key();
#else
	return -1;
#endif
}

void LG_display_image() {
#ifdef LG_USE_FPT
	G_display_image();
#endif
}

void LG_save_image_to_file(char *file_name) {
#ifdef LG_USE_FPT
	G_save_image_to_file(file_name);
#else
	xwd_map_to_named_xwd_file(LG_WINDOW_ID, file_name) ;
#endif
}
