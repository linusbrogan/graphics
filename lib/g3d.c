#include <sys/stat.h>
#include <sys/types.h>
#include <graphics.h>
#include <light_model.h>
#include <lg.h>

#define RESOLUTION 1000
#define WS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define WINDOW_SIZE (WS_MAX(WINDOW_WIDTH, WINDOW_HEIGHT))
#define DX (-((WINDOW_SIZE) - (WINDOW_WIDTH)) / 2)
#define DY (-((WINDOW_SIZE) - (WINDOW_HEIGHT)) / 2)
#define HALF_ANGLE (M_PI / 6)
#define H (tan(HALF_ANGLE))
#define HITHER (1e-3)
#define YON (1e50)

double Z_BUFFER[WINDOW_WIDTH][WINDOW_HEIGHT];

void initialize_z_buffer() {
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			Z_BUFFER[x][y] = YON;
		}
	}
}

int project(double w, double z, enum coordinate sign) {
	double w_bar = w / z;
	double w_prime = (w_bar / H + 1) * WINDOW_SIZE / 2;
	int w_px = round(w_prime);
	if (sign == _X) w_px += DX;
	if (sign == _Y) w_px += DY;
	return w_px;
}

void set_color(
	int (*f_xyz)(double, double, double[3]),
	double u,
	double du,
	double v,
	double dv,
	double T[4][4],
	void (*inherent_rgb)(double, double, double *)
) {
	double P[3];
	f_xyz(u, v, P);
	double u2 = u + du;
	double Q[3];
	f_xyz(u2, v, Q);
	double v2 = v + dv;
	double R[3];
	f_xyz(u, v2, R);

	M3d_mat_mult_pt(P, T, P);
	M3d_mat_mult_pt(Q, T, Q);
	M3d_mat_mult_pt(R, T, R);

	double A[3];
	double B[3];
	for (int i = 0; i < 3; i++) {
		A[i] = Q[i] - P[i];
		B[i] = R[i] - P[i];
	}

	double n[3];
	M3d_x_product(n, A, B);

	double eye[3] = {0, 0, 0};
	double irgb[3];
	inherent_rgb(u, v, irgb);
	double rgb[3];
	Light_Model(irgb, eye, P, n, rgb);
	LG_rgb(rgb[_R], rgb[_G], rgb[_B]);
}

void graph_3d(
	int (*f_xyz)(double, double, double[3]),
	double u_start,
	double u_end,
	double v_start,
	double v_end,
	double T[4][4],
	void (*rgb)(double, double, double *)
) {
	double du = (u_end - u_start) / RESOLUTION;
	double dv = (v_end - v_start) / RESOLUTION;
	for (double u = u_start; u <= u_end; u += du) {
		for (double v = v_start; v <= v_end; v += dv) {
			double point[3];
			f_xyz(u, v, point);
			M3d_mat_mult_pt(point, T, point);
			double z = point[_Z];
			int x = project(point[_X], z, _X);
			int y = project(point[_Y], z, _Y);
			if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
				if (z > HITHER && z < Z_BUFFER[x][y]) {
					Z_BUFFER[x][y] = z;
					set_color(f_xyz, u, du, v, dv, T, rgb);
					LG_point(x, y);
				}
			}
		}
	}
}
