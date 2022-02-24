double sphere_x(double u, double v) {
	return cos(u) * cos(v);
}

double sphere_y(double u, double v) {
	return sin(u) * cos(v);
}

double sphere_z(double u, double v) {
	return sin(v);
}

#define R1 (0.2)
#define R2 (0.4)

double cylinder_x(double u, double v) {
	double r = 1;
	if (fabs(v) < R1) r = 1;
	else if (fabs(v) < R2) r = 1 + (fabs(v) - R1) / (R2 - R1);
	else r = 2;
	return r * cos(u);
}

double cylinder_y(double u, double v) {
	double r = 1;
	if (fabs(v) < R1) r = 1;
	else if (fabs(v) < R2) r = (fabs(v) - R1) / (R2 - R1) + 1;
	else r = 2;
	return r * sin(u);
}

double cylinder_z(double u, double v) {
	return v;
}

double snorus_xyz(double u, double v, double *xyz) {
	double square[3] = {square2_x(u), square2_y(u), 0};
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double M[4][4];
	double _i[4][4];

	T_type[T_n] = RZ;	T_param[T_n] = 45;	T_n++;
	T_type[T_n] = TY;	T_param[T_n] = 3;	T_n++;
	T_type[T_n] = RX;	T_param[T_n] = v;	T_n++;
	M3d_make_movement_sequence_matrix(M, _i, T_n, T_type, T_param);
	M3d_mat_mult_pt(xyz, M, square);
}

double snorus_x(double u, double v) {
	double xyz[3];
	snorus_xyz(u, v, xyz);
	return xyz[0];
}

double snorus_y(double u, double v) {
	double xyz[3];
	snorus_xyz(u, v, xyz);
	return xyz[1];
}

double snorus_z(double u, double v) {
	double xyz[3];
	snorus_xyz(u, v, xyz);
	return xyz[2];
}