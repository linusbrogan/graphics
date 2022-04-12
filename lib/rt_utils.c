#include <math.h>
#include <m3d.h>

double sq(double x) {
	return x * x;
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

int solve_quadratic(double a, double b, double c, double x[2]) {
	double root = b * b - 4 * a * c;
	if (root < 0) return 0;
	root = sqrt(root);
	x[0] = (-b - root) / (2 * a);
	x[1] = (-b + root) / (2 * a);
	if (root == 0) return 1;
	return 2;
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

int find_refraction_vector(double n1, double n2, double normal[3], double v1[3], double v2[3]) {
	double c1 = -dot_product(v1, normal) / sqrt(dot_product(v1, v1) * dot_product(normal, normal));
	double sign = sgn(c1); // +1 if normal is on same side as incident vector v1, otherwise -1
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
	for (int i = 0; i < 3; i++) {
		X[2] *= -sign;
	}
	double phi = theta1 - theta2;

	double eye[3] = {0, 0, 0}; // Rotating vectors, so center at origin
	double *coi = v1; // Just needs to be in the plane span(v1,normal)
	double *up = X; // Cross product points up
	double V[4][4];
	double V_i[4][4];
	M3d_view_(M, M_i, eye, coi, up);
	double R[4][4;
	M3d_make_y_rotation_cs(R, cos(phi), sin(phi));
	double M[4][4];
	M3d_mat_mult(M, V_i, R);
	M3d_mat_mult(M, M, V);
	M3d_mat_mult_pt(v2, M, v1);
	return 1;
}
