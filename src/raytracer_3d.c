#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <raytracing.h>

#define OUTPUT_PATH "out/Raytracer_3D"
#define FRAMES 100

void initialize() {
	srand48(&initialize);
	LG_init_graphics(1080 / 2, 720 / 2);
	initialize_texture_maps();
	HALF_ANGLE = M_PI / 4;

	// Create frame directory
	mkdir(OUTPUT_PATH, 0777);
}

void save_image(int frame, char *prefix) {
	char file_name[100];
	sprintf(file_name, "%s/%s_%04d.xwd", OUTPUT_PATH, prefix, frame);
	LG_display_image();
	LG_save_image_to_file(file_name);
}

void apply_transformation(int object_start, int object_end, double T[4][4], double T_i[4][4]) {
	for (int i = object_start; i < object_end; i++) {
		M3d_mat_mult(object_matrix[i], T, object_matrix[i]);
		M3d_mat_mult(object_matrix_i[i], object_matrix_i[i], T_i);
	}
}

int main(int argc, char *argv[]) {
	int frame_start = 0;
	int frame_stop = FRAMES;
	if (argc >= 2) {
		frame_start = atoi(argv[1]);
	}
	if (argc >= 3) {
		frame_stop = atoi(argv[2]);
	}

	initialize();

	// Create movement sequence
	int T_n = 0;
	int T_type[100];
	double T_param[100];
	double M[4][4];
	double M_i[4][4];

	// Make bubbles
	int num_bubbles = 50;
	double bubbles[3 * num_bubbles];
	for (int i = 0; i < 3 * num_bubbles; i++) {
		bubbles[i] = drand48();
	}

	int frame = frame_start;
	int side = -1;
	while (1) {
		if (LG_no_wait_key() == 'q' || frame >= frame_stop)
			return 0;

		printf("Rendering frame %04d\n", frame);

		// Reset frame
		LG_rgb(0, 0, 0);
		LG_clear();
		clear_objects();

		// Configure frame
		double t = frame * TAU / FRAMES;
		double eye[3] = {2 + 4 * cos(2 * t), 2 + 5 * sin(2 * t), 2 + 0.5 * sin(5 * t)};
		double coi[3] = {0, 0, 2};
		double up[3] = {eye[_X], eye[_Y], eye[_Z] + 1};
		double eye_spacing = 1;

		// Make view matrix
		double view[4][4];
		double view_i[4][4];
		M3d_view_3d(view, view_i, eye, coi, up, eye_spacing * side);

		// Set light
		double light[3] = {-10, 10, 10};
		M3d_mat_mult_pt(light_in_eye_space, view, light);

		// Parameters
		double glass_inner_radius = 1;
		double glass_thickness = 0.05;
		double glass_height = 3;
		double water_height = glass_height * 0.6;
		double water_radius = glass_inner_radius * 0.99;
		double n_glass = 1.5;
		double n_water = 1.33;

		// Build glass
		double glass_object_start = objects;
		// Inner wall
		object_type[objects] = OBJ_CYLINDER;
		object_texture[objects] = TM_SOLID_COLOR;
		object_color[objects][_R] = 0.5;
		object_color[objects][_G] = 1;
		object_color[objects][_B] = 0.8;
		object_reflectivity[objects] = 0.2;
		object_opacity[objects] = 0.1;
		object_refractive_index[objects] = n_glass;
		T_n = 0;
//!!		T_type[T_n] = NX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = glass_inner_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = glass_inner_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = (glass_height - glass_thickness) / 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = glass_thickness;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
//		M3d_mat_mult(object_matrix[objects], view, M);
//		M3d_mat_mult(object_matrix_i[objects], M_i, view_i);
		objects++;

		// Outer wall
		object_type[objects] = OBJ_CYLINDER;
		object_texture[objects] = TM_SOLID_COLOR;
		for (int i = 0; i < 3; i++)
			object_color[objects][i] = object_color[objects - 1][i];
		object_reflectivity[objects] = object_reflectivity[objects - 1];
		object_opacity[objects] = object_opacity[objects - 1];
		object_refractive_index[objects] = object_refractive_index[objects - 1];
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = glass_inner_radius + glass_thickness;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = glass_inner_radius + glass_thickness;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = glass_height / 2;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
//!!		objects++;

		// Bottom wall
		double bottom_wall_params[2] = {0, glass_inner_radius + glass_thickness};
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = bottom_wall_params;
		object_texture[objects] = TM_SOLID_COLOR;
		for (int i = 0; i < 3; i++)
			object_color[objects][i] = object_color[objects - 1][i];
		object_reflectivity[objects] = object_reflectivity[objects - 1];
		object_opacity[objects] = object_opacity[objects - 1];
		object_refractive_index[objects] = object_refractive_index[objects - 1];
		T_n = 0;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Inner bottom wall
		double inner_bottom_wall_params[2] = {0, glass_inner_radius};
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = inner_bottom_wall_params;
		object_texture[objects] = TM_SOLID_COLOR;
		for (int i = 0; i < 3; i++)
			object_color[objects][i] = object_color[objects - 1][i];
		object_reflectivity[objects] = object_reflectivity[objects - 1];
		object_opacity[objects] = object_opacity[objects - 1];
		object_refractive_index[objects] = object_refractive_index[objects - 1];
		T_n = 0;
		T_type[T_n] = NZ;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = glass_thickness;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Water
		object_type[objects] = OBJ_CYLINDER;
		object_texture[objects] = TM_SOLID_COLOR;
		object_color[objects][_R] = 0.64;
		object_color[objects][_G] = 0.17;
		object_color[objects][_B] = 0.17;
		object_reflectivity[objects] = 0.2;
		object_opacity[objects] = 0.2;
		object_refractive_index[objects] = n_water;
		T_n = 0;
		//T_type[T_n] = NX;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = water_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = water_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = water_height / 2;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = glass_thickness;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Water top
		double water_disk[2] = {0, water_radius};
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = water_disk;
		object_texture[objects] = TM_SOLID_COLOR;
		object_color[objects][_R] = 0.64;
		object_color[objects][_G] = 0.17;
		object_color[objects][_B] = 0.17;
		object_reflectivity[objects] = 0.2;
		object_opacity[objects] = 0.2;
		object_refractive_index[objects] = n_water;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = water_height;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Water bottom
		object_type[objects] = OBJ_ANNULUS;
		object_parameters[objects] = water_disk;
		object_texture[objects] = TM_SOLID_COLOR;
		object_color[objects][_R] = 0.64;
		object_color[objects][_G] = 0.17;
		object_color[objects][_B] = 0.17;
		object_reflectivity[objects] = 0.2;
		object_opacity[objects] = 0.2;
		object_refractive_index[objects] = n_water;
		T_n = 0;
		T_type[T_n] = NZ;	T_param[T_n] = -1;	T_n++;
		T_type[T_n] = TZ;	T_param[T_n] = glass_thickness;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build bubbles
		double bubble_radius = 0.01;
		for (int i = 0; i < num_bubbles; i++) {
			object_type[objects] = OBJ_SPHERE;
			object_texture[objects] = TM_SOLID_COLOR;
			object_color[objects][_R] = 1;
			object_color[objects][_G] = 1;
			object_color[objects][_B] = 1;
			object_reflectivity[objects] = 0.2;
			object_opacity[objects] = 0.7;
			T_n = 0;
			double bx = bubbles[i * 3] * glass_inner_radius * cos(TAU * bubbles[i * 3 + 1]);
			double by = bubbles[i * 3] * glass_inner_radius * sin(TAU * bubbles[i * 3 + 1]);
			T_type[T_n] = SX;	T_param[T_n] = bubble_radius;	T_n++;
			T_type[T_n] = SY;	T_param[T_n] = bubble_radius;	T_n++;
			T_type[T_n] = SZ;	T_param[T_n] = bubble_radius;	T_n++;
			T_type[T_n] = TX;	T_param[T_n] = bx;	T_n++;
			T_type[T_n] = TY;	T_param[T_n] = by;	T_n++;
			T_type[T_n] = TZ;	T_param[T_n] = fmod((bubbles[i * 3 + 2] + t / TAU), 1) * water_height;	T_n++;
			M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
			objects++;
		}

		// Build straw
		object_type[objects] = OBJ_CYLINDER;
		object_texture[objects] = TM_SOLID_COLOR;
		object_color[objects][_R] = 1;
		object_color[objects][_G] = 0.2;
		object_color[objects][_B] = 0.2;
		object_reflectivity[objects] = 0.1;
		object_opacity[objects] = 0.8;
		object_refractive_index[objects] = 1.4;
		T_n = 0;
		double straw_radius = glass_inner_radius / 15;
		double straw_height = glass_height * 1.5;
		T_type[T_n] = TZ;	T_param[T_n] = 1;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = straw_radius;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = straw_radius;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = straw_height / 2;	T_n++;
		T_type[T_n] = TY;	T_param[T_n] = straw_radius;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		double straw_eye[3] = {0, glass_inner_radius - 2 * straw_radius, glass_thickness * 1.5};
		double straw_coi[3] = {0, -glass_inner_radius + 2 * straw_radius, glass_height + straw_radius};
		double straw_up[3] = {0, 0, 0};
		M3d_view(M, M_i, straw_eye, straw_coi, straw_up);
		apply_transformation(objects, objects + 1, M_i, M);
		objects++;

		double glass_object_end = objects;
		//apply_transformation(glass_object_start, glass_object_end, view, view_i);


//bad
		// Build donut
		object_type[objects] = OBJ_TORUS;
		object_texture[objects] = TM_CHECKERBOARD;
		object_reflectivity[objects] = 0.1;
		T_n = 0;
		T_type[T_n] = TZ;	T_param[T_n] = 0.2;	T_n++;
		T_type[T_n] = SX;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 0.3;	T_n++;
		T_type[T_n] = RX;	T_param[T_n] = -32;	T_n++;
		T_type[T_n] = TX;	T_param[T_n] = 1;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build table
		object_type[objects] = OBJ_PLANE;
		object_texture[objects] = TM_CHECKERBOARD;
		T_n = 0;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		objects++;

		// Build BG
		object_type[objects] = OBJ_SPHERE;
		object_texture[objects] = TM_CHECKERBOARD;
		T_n = 0;
		T_type[T_n] = SX;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = SY;	T_param[T_n] = 10000;	T_n++;
		T_type[T_n] = SZ;	T_param[T_n] = 10000;	T_n++;
		M3d_make_movement_sequence_matrix(object_matrix[objects], object_matrix_i[objects], T_n, T_type, T_param);
		//objects++;

		apply_transformation(0, objects, view, view_i);
		render();

//		if (side < 0) {
			save_image(frame, "left");
//			side = 1;
//		} else {
//			save_image(frame, "right");
//			side = -1;
			frame++;
//		}
	}

	return 0;
}
