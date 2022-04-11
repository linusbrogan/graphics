#include <math.h>
#include <m3d.h>
#include <polynomial.h>

int sign(double x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

double dot_product(double v[3], double w[3]) {
	double sum = 0;
	for (int i = 0; i < 3; i++) {
		sum += v[i] * w[i];
	}
	return sum;
}

void normalize(double v[3]) {
	double length = sqrt(dot_product(v, v));
	if (length == 0) return;
	for (int i = 0; i < 3; i++) {
		v[i] /= length;
	}
}

void find_reflection_vector(double incident[3], double normal[3], double reflected[3]) {
	double l[3];
	double n[3];
	for (int i = 0; i < 3; i++) {
		l[i] = -incident[i];
		n[i] = normal[i];
	}
	normalize(l);
	normalize(n);
	double scale = 2 * dot_product(l, n);
	for (int i = 0; i < 3; i++) {
		reflected[i] = scale * n[i] - l[i];
	}
}

int find_refraction_vector(double n1, double n2, double normal[3], double v1[3], double v2[3]) {
	double c1 = -dot_product(v1, normal) / sqrt(dot_product(v1, v1) * dot_product(normal, normal));
	double normal_sign = sign(c1); // +1 if normal is on same side as incident vector v1, otherwise -1
	c1 = fabs(c1);
	if (c1 >= 1 - EPSILON) {
		for (int i = 0; i < 3; i++) {
			v2[i] = v1[i];
		}
		return 1;
	}
	double theta1 = acos(c1);
	double s2 = sin(theta1) * n1 / n2;
	if (s2 >= 1 - EPSILON) return 0;
	double theta2 = asin(s2);

	double X[3];
	M3d_x_product(X, v1, normal);
	X[2] *= -normal_sign;
	double phi = sign(X[2]) * (theta1 - theta2);

	double M[4][4];
	M3d_make_z_rotation_cs(M, cos(phi), sin(phi));
	M3d_mat_mult_pt(v2, M, v1);
	return 1;
}

int orient_normal(double intersection[3], double normal[3], double eye[3]) {
	// Find vector from intersection point to eye
	double e[3];
	for (int i = 0; i < 3; i++) {
		e[i] = eye[i] - intersection[i];
	}

	// Orient the normal on the same side as the eye
	double dot = dot_product(normal, e);
	double normal_sign = 1;
	if (dot < 0) {
		normal_sign = -1;
		for (int i = 0; i < 3; i++) {
			normal[i] *= -1;
		}
	}

	normalize(normal);
	return normal_sign;
}
