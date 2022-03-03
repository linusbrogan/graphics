#include <math.h>
#include <m3d.h>
#include <graphics.h>
#include <shape_2d.h>

int sphere_xyz(double u, double v, double xyz[3]) {
	xyz[_X] = cos(u) * cos(v);
	xyz[_Y] = sin(u) * cos(v);
	xyz[_Z] = sin(v);
	return 1;
}

int cylinder_xyz(double u, double v, double xyz[3]) {
	xyz[_X] = cos(u);
	xyz[_Y] = sin(u);
	xyz[_Z] = v;
	return 1;
}

int disk_xyz(double radius, double theta, double xyz[3]) {
	xyz[_X] = radius * cos(theta);
	xyz[_Y] = radius * sin(theta);
	xyz[_Z] = 0;
	return 1;
}

int space_station_central_axis_xyz(double u, double v, double xyz[3]) {
	double R1 = 0.2;
	double R2 = 0.4;
	int rc = cylinder_xyz(u, v, xyz);
	double r = 1;
	if (fabs(v) < R1) r = 1;
	else if (fabs(v) < R2) r += (fabs(v) - R1) / (R2 - R1);
	else r++;
	xyz[_X] *= r;
	xyz[_Y] *= r;
	return 1;
}

int torus_xyz(double u, double v, double xyz[3]) {
	double r = 0.1;
	double R = 1;
	double R_prime = R + r * cos(v);
	xyz[_X] = R_prime * cos(u);
	xyz[_Y] = R_prime * sin(u);
	xyz[_Z] = r * sin(v);
	return 1;
}

int space_station_ring_xyz(double u, double v, double xyz[3]) {
	double P[3] = {square_x(v), square_y(v), 0};
	int T_type[10];
	double T_param[10];
	int T_n = 0;
	T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
	T_type[T_n] = SX;	T_param[T_n] = 0.1 * sqrt(2);	T_n++;
	T_type[T_n] = SY;	T_param[T_n] = 0.1 * sqrt(2);	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 1;	T_n++;
	T_type[T_n] = RY;	T_param[T_n] = 90;	T_n++;
	T_type[T_n] = RZ;	T_param[T_n] = u / DEGREES;	T_n++;
	double M[4][4];
	double M_i[4][4];
	M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
	M3d_mat_mult_pt(xyz, M, P);
	return 1;
}
