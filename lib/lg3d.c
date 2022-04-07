// Graphics library shims for FPTXWD Tools with 3D

#include <fpt.h>
#include <graphics.h>
#include <xwd.h>

int LG_WINDOW_WIDTH = 0;
int LG_WINDOW_HEIGHT = 0;

int LG_WINDOW_ID = -1;
int LG_WINDOW_ID_LEFT = -1;
int LG_WINDOW_ID_RIGHT = -1;
int LG_INITIALIZED = 0;
double LG_RGB[3] = {0, 0, 0};

void LG3d_left() {
	LG_WINDOW_ID = LG_WINDOW_ID_LEFT;
}

void LG3d_right() {
	LG_WINDOW_ID = LG_WINDOW_ID_RIGHT;
}

void LG_init_graphics(int width, int height) {
	LG_WINDOW_WIDTH = width;
	LG_WINDOW_HEIGHT = height;
	G_init_graphics(width, height);
	if (!LG_INITIALIZED) {
		LG_INITIALIZED = 1;
		LG_WINDOW_ID_LEFT = create_new_xwd_map(width, height);
		LG_WINDOW_ID_RIGHT = create_new_xwd_map(width, height);
		LG3d_left();
	}
	if (LG_WINDOW_ID_LEFT == -1 || LG_WINDOW_ID_RIGHT == -1) {
		fprintf(stderr, "Could not initialize XWD map.");
		exit(-1);
	}
}

void LG_rgb(double r, double g, double b) {
	G_rgb(r, g, b);
	LG_RGB[_R] = r;
	LG_RGB[_G] = g;
	LG_RGB[_B] = b;
}

void LG_point(int x, int y) {
	G_point(x, y);
	double r = LG_RGB[_R];
	double g = LG_RGB[_G];
	double b = LG_RGB[_B];
	set_xwd_map_color(LG_WINDOW_ID, x, y, r, g, b);
}

void LG_clear() {
	G_clear();
	for (int x = 0; x < LG_WINDOW_WIDTH; x++) {
		for (int y = 0; y < LG_WINDOW_WIDTH; y++) {
			LG_point(x, y);
		}
	}
}

int LG_no_wait_key() {
	return G_no_wait_key();
}

void LG_display_image() {
	G_display_image();
}

void LG_save_image_to_file(char *file_name) {
	G_save_image_to_file(file_name);
}

void LG3d_color_filter() {
	for (int x = 0; x < LG_WINDOW_WIDTH; x++) {
		for (int y = 0; y < LG_WINDOW_HEIGHT; y++) {
			double rgb_l[3];
			int error = get_xwd_map_color(LG_WINDOW_ID_LEFT, x, y, rgb_l);
			rgb_l[_G] = 0;
			rgb_l[_B] = 0;

			double rgb_r[3];
			error = get_xwd_map_color(LG_WINDOW_ID_RIGHT, x, y, rgb_r);
			rgb_r[_R] = 0;

			double rgb[3];
			for (int i = 0; i < 3; i++) {
				rgb[i] = (rgb_l[i] + rgb_r[i]);
			}

			G_rgb(rgb[_R], rgb[_G], rgb[_B]);
			G_point(x, y);
		}
	}

}

void LG3d_merge() {
	LG3d_color_filter();
LG_display_image();return;
	for (int x = 0; x < LG_WINDOW_WIDTH; x++) {
		for (int y = 0; y < LG_WINDOW_HEIGHT; y++) {
			double rgb_l[3];
			double rgb_r[3];
			int error_l = get_xwd_map_color(LG_WINDOW_ID_LEFT, x, y, rgb_l);
			int error_r = get_xwd_map_color(LG_WINDOW_ID_RIGHT, x, y, rgb_r);
			if (error_l == -1 || error_r == -1) {
				fprintf(stderr, "Failed to load image color.");
			}
			double rgb[3];
			for (int i = 0; i < 3; i++) {
				rgb[i] = (rgb_l[i] + rgb_r[i]) / 2;
			}
			LG_rgb(rgb[_R], rgb[_G], rgb[_B]);
			G_point(x, y);
		}
	}
	LG_display_image();
}
