double sphere_x(double u, double v) {
	return cos(u) * cos(v);
}

double sphere_y(double u, double v) {
	return sin(u) * cos(v);
}

double sphere_z(double u, double v) {
	return sin(v);
}

double cylinder_x(double u, double v) {
	double r = 1;
	if (fabs(v) < 0.3) r = 1;
	else if (fabs(v) < 0.6) r = (0.6 - fabs(v)) / (0.6 - 0.3) + 1;
	else r = 2;
	return r * cos(u);
}

double cylinder_y(double u, double v) {
	double r = 1;
	if (fabs(v) < 0.3) r = 1;
	else if (fabs(v) < 0.6) r = (0.6 - fabs(v)) / (0.6 - 0.3) + 1;
	else r = 2;
	return r * sin(u);
}

double cylinder_z(double u, double v) {
	return v;
}
