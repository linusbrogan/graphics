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
