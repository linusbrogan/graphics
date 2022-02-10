double (*circle_x)(double) = cos;
double (*circle_y)(double) = sin;

double id(double x) {
	return x;
}

double (*sum4_x)(double) = id;
double sum4_y(double u) {
	return pow(1 - u * u * u * u, 0.25);
}

double square_x(double u) {
	if (u >= 0 && u < 1) {
		double w = u;
		double x = 1 - w;
		return x;
	}
	if (u >= 1 && u < 2) {
		double w = u - 1;
		double x = -w;
		return x;
	}
	if (u >= 2 && u < 3) {
		double w = u - 2;
		double x = w - 1;
		return x;
	}
	if (u >= 3 && u <= 4) {
		double w = u - 3;
		double x = w;
		return x;
	}
	return 0;
}

double square_y(double u) {
	if (u >= 0 && u < 1) {
		double w = u;
		double y = w;
		return y;
	}
	if (u >= 1 && u < 2) {
		double w = u - 1;
		double y = 1 - w;
		return y;
	}
	if (u >= 2 && u < 3) {
		double w = u - 2;
		double y = -w;
		return y;
	}
	if (u >= 3 && u <= 4) {
		double w = u - 3;
		double y = w - 1;
		return y;
	}
	return 0;
}

double sgn(double x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

double square2_x(double u) {
	return sgn(cos(u)) * pow(cos(u), 2);
}

double square2_y(double u) {
	return sgn(sin(u)) * pow(sin(u), 2);
}

double astroid_x(double u) {
	return sgn(cos(u)) * pow(cos(u), 4);
}

double astroid_y(double u) {
	return sgn(sin(u)) * pow(sin(u), 4);
}

double (*hyperbola_x)(double) = cosh;
double (*hyperbola_y)(double) = sinh;

double (*parabola_x)(double) = id;
double parabola_y(double u) {
	return u * u;
}

double lemon_x(double u) {
	return pow(cos(u), 3);
}

double (*lemon_y)(double) = sin;


double brachistochrone_x(double u) {
	return u - sin(u);
}

double brachistochrone_y(double u) {
	return 1 - cos(u);
}
