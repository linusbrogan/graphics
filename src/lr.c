#include <fpt.h>
#include <graphics.h>
#include <xwd.h>

int main() {
	int id_l = init_xwd_map_from_file("left.xwd");
	if (id_l == -1) {
		fprintf(stderr, "Failed to load left.xwd");
		return 1;
	}
	int d_l[2];
	int error = get_xwd_map_dimensions(id_l, d_l);
	if (error == -1) {
		fprintf(stderr, "Failed to get left dimensions");
		return 1;
	}

	int id_r = init_xwd_map_from_file("right.xwd");
	if (id_r == -1) {
		fprintf(stderr, "Failed to load right.xwd");
		return 1;
	}
	int d_r[2];
	error = get_xwd_map_dimensions(id_r, d_r);
	if (error == -1) {
		fprintf(stderr, "Failed to get right dimensions");
		return 1;
	}

	if (d_l[0] != d_r[0] || d_l[1] != d_r[1]) {
		fprintf(stderr, "Images have different dimensions");
	}

	G_init_graphics(d_l[0], d_l[1]);
	for (int x = 0; x < d_l[0]; x++) {
		for (int y = 0; y < d_l[1]; y++) {
			double rgb_l[3];
			error = get_xwd_map_color(id_l, x, y, rgb_l);
			rgb_l[_G] *= 0.5;
			rgb_l[_B] *= 0.5;

			double rgb_r[3];
			error = get_xwd_map_color(id_r, x, y, rgb_r);
			rgb_r[_R] *= 0.5;

			double rgb[3];
			for (int i = 0; i < 3; i++) {
				rgb[i] = (rgb_l[i] + rgb_r[i]) / 2;
			}

			G_rgb(rgb[_R], rgb[_G], rgb[_B]);
			G_point(x, y);
		}
	}
	G_wait_key();
}
