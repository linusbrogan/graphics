#include <sys/stat.h>
#include <sys/types.h>
#include <fpt.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define FILE_NAME "out/3d_Simple_RaytracerDS.xwd"

double dLR = 0;
double ddLR = 1;

void initialize() {
	initialize_texture_maps();
	LG_init_graphics(800, 800);
	HALF_ANGLE = 40 * DEGREES;
}

void handle_keys() {
	while (1) {
		int key = G_wait_key();
		switch (key) {
			case 'd':
			printf("display\n");
			return;

			case 'm':
			dLR += ddLR;
			printf("increase dLR by %lf to %lf\n", ddLR, dLR);
			break;

			case 'n':
			dLR -= ddLR;
			if (dLR < 0) dLR = 0;
			printf("decrease dLR by %lf to %lf\n", ddLR, dLR);
			break;

			case 'o':
			ddLR /= 10;
			printf("decrease ddLR by 10x to %lf\n", ddLR);
			break;

			case 'p':
			ddLR *= 10;
			printf("increase ddLR by 10x to %lf\n", ddLR);
			break;

			case 'q':
			printf("quit\n");
			exit(0);

			case 's':
			printf("save\n");
			LG_save_image_to_file(FILE_NAME);
		}
	}
}

int main() {
	initialize();

	int side = -1;
	while (1) {
		if (side < 0) LG3d_left();
		else LG3d_right();

		// Create movement sequence
		int T_n = 0;
		int T_type[100];
		double T_param[100];
		double M[4][4];
		double M_i[4][4];

		// Reset frame
		LG_rgb(0, 0, 0);
		LG_clear();
		objects = 0;

		// Configure frame
		double eye[3] = {0, 400, -1000};
		double coi[3] = {0, 0, 0};
		double up[3] = {eye[_X], eye[_Y] + 1, eye[_Z]};

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view_3d(view, view_i, eye, coi, up, dLR * side);

		// Set light
		double light_in_world_space[3] = {100, 300, 50};
		M3d_mat_mult_pt(light_in_eye_space, view, light_in_world_space);

		// Build objects
		object_type[objects] = OBJ_SPHERE;
		object_color[objects][_R] = 0.8;
		object_color[objects][_G] = 0.8;
		object_color[objects][_B] = 0.8;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 800;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 800;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = -150;	T_n++;
		M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
		M3d_mat_mult(object_matrix[objects], view, M);
		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		for (int i = 0; i < 3; i++) {
			object_type[objects] = OBJ_SPHERE;
			object_color[objects][_R] = 1;
			object_color[objects][_G] = 0.8;
			object_color[objects][_B] = 0.8;
			object_reflectivity[objects] = 0;
			object_opacity[objects] = 1;
			object_texture[objects] = TM_SOLID_COLOR;
			T_n = 0;
			T_type[T_n] = SX;	T_param[T_n] = 500;	T_n++;
			T_type[T_n] = SY;	T_param[T_n] = 100;	T_n++;
			T_type[T_n] = SZ;	T_param[T_n] = 80;	T_n++;
			T_type[T_n] = RY;	T_param[T_n] = i * 360 / 3;	T_n++;
			T_type[T_n] = TY;	T_param[T_n] = 650;	T_n++;
			M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
			M3d_mat_mult(object_matrix[objects], view, M);
			M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
			objects++;
		}

		for (int i = 0; i < 6; i++) {
			object_type[objects] = OBJ_HYPERBOLOID;
			object_color[objects][_R] = 0.8;
			object_color[objects][_G] = 0.4;
			object_color[objects][_B] = 0.2;
			object_reflectivity[objects] = 0;
			object_opacity[objects] = 1;
			object_texture[objects] = TM_SOLID_COLOR;
			T_n = 0;
			T_type[T_n] = SX;	T_param[T_n] = 40;	T_n++;
			T_type[T_n] = SY;	T_param[T_n] = 150;	T_n++;
			T_type[T_n] = SZ;	T_param[T_n] = 40;	T_n++;
			T_type[T_n] = TX;	T_param[T_n] = 600;	T_n++;
			T_type[T_n] = RY;	T_param[T_n] = i * 360 / 6;	T_n++;
			M3d_make_movement_sequence_matrix(M, M_i, T_n, T_type, T_param);
			M3d_mat_mult(object_matrix[objects], view, M);
			M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
			objects++;
		}

		render();
		if (side > 0) {
			LG3d_merge();
			handle_keys();
		}
		side *= -1;
	}

	return 0;
}
