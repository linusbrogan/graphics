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
	return cos(u);
}

double cylinder_y(double u, double v) {
	return sin(u);
}

double cylinder_z(double u, double v) {
	return v;
}
