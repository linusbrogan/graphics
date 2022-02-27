enum coordinate {
	_X = 0,
	_Y,
	_Z
};

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
