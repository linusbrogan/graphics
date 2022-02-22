enum texture_map_id {
	TM_CHECKERBOARD = 0,
	TM_CLOCK,
	TM_MANDELBROT_SET
};
char *TEXTURE_MAP_FILES[] = {
	"checkerboard.xwd",
	"XWD_TOOLS_03/clock.xwd",
	"XWD_TOOLS_03/mandelbrot_set.xwd"
};
int TEXTURE_MAPS = 3;
int TEXTURE_IDS[100];

void initialize_texture_maps() {
	for (int i = 0; i < TEXTURE_MAPS; i++) {
		TEXTURE_IDS[i] = init_xwd_map_from_file(TEXTURE_MAP_FILES[i]);
		if (TEXTURE_IDS[i] == -1) {
			fprintf(stderr, "Failed to load texture %d\n", i);
		}
	}
}

void clock_map(double u, double v, double *rgb) {
	double u_start = 0-1;
	double u_end = 1;// * M_PI;
	double v_start = -1;//- M_PI / 2;
	double v_end = 1;//M_PI / 2;
	int d[2];
	int error = get_xwd_map_dimensions(TEXTURE_IDS[TM_CLOCK], d);
	if (error == -1) return;
	int x = (u - u_start) / (u_end - u_start) * d[0];
	int y = (v - v_start) / (v_end - v_start) * d[1];
	get_xwd_map_color(TEXTURE_IDS[TM_CLOCK], x, y, rgb);
}


void unit_checkerboard_map(double u, double v, double *rgb) {
	double uu = fmod(u, 2) + 2;
	double vv = fmod(v, 2) + 2;
	int a = fmod(uu, 2) < 1;
	int b = fmod(vv, 2) < 1;
	int c = a != b;
	rgb[0] = c ? 0.7 : 0.3;
	rgb[1] = rgb[0];
	rgb[2] = rgb[0];
}

void (*origin_rgb)(double, double, double *) = unit_checkerboard_map;

void origin_rgb_original(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 0.8;
	rgb[2] = 0;
}

void x_rgb(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 0.2;
	rgb[2] = 0.2;

}

void y_rgb(double u, double v, double *rgb) {
	rgb[0] = 1;
	rgb[1] = 1;
	rgb[2] = 1;
}

void z_rgb(double u, double v, double *rgb) {
	rgb[0] = 0.3;
	rgb[1] = 0.2;
	rgb[2] = 1;

}

void diagonal_rgb(double u, double v, double *rgb) {
	rgb[0] = 0;
	rgb[1] = 0.5;
	rgb[2] = 0.5;
}
