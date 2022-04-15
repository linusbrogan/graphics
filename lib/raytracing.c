#include <lg.h>
#include <light_model.h>
#include <m3d.h>
#include <rt_shapes.h>
#include <rt_utils.h>
#include <textures.h>

#define MAXIMUM_OBJECTS 100
#define MAXIMUM_REFLECTIONS 4
#define MINIMUM_INTENSITY 0.05
#define INTENSITY_DECAY 0.99
#define WS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define WINDOW_SIZE (WS_MAX(LG_WINDOW_WIDTH, LG_WINDOW_HEIGHT))
double HALF_ANGLE = (M_PI / 6);
#define H (tan(HALF_ANGLE))
#define EPSILON (1e-10)

enum object_type object_type[MAXIMUM_OBJECTS];
void *object_parameters[MAXIMUM_OBJECTS];
double object_matrix[MAXIMUM_OBJECTS][4][4];
double object_matrix_i[MAXIMUM_OBJECTS][4][4];
double object_color[MAXIMUM_OBJECTS][3];
double object_reflectivity[MAXIMUM_OBJECTS];
double object_opacity[MAXIMUM_OBJECTS];
enum texture_map object_texture[MAXIMUM_OBJECTS];
int objects = MAXIMUM_OBJECTS;

void clear_objects() {
	for (int i = 0; i < objects; i++) {
		object_type[i] = -1;
		object_parameters[i] = NULL;
		object_reflectivity[i] = 0;
		object_opacity[i] = 1;
		object_texture[i] = TM_SOLID_COLOR;
	}
	objects = 0;
}

void matrix_copy(double from[4][4], double to[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			to[i][j] = from[i][j];
		}
	}
}

void configure_xyz_shape(enum xyz_shape shape, double M[4][4], double M_i[4][4]) {
	int model = objects;
	while (1) {
		object_type[objects] = OBJ_TRIANGLE;
		matrix_copy(M, object_matrix[objects]);
		matrix_copy(M_i, object_matrix_i[objects]);
		for (int i = 0; i < 3; i++)
			object_color[objects][i] = object_color[model][i];
		object_reflectivity[objects] = object_reflectivity[model];
		object_opacity[objects] = object_opacity[model];
		object_texture[objects] = object_texture[model];
		object_parameters[objects] = NULL;
		objects++
	}
}

void shape_texture_map(int object, double xyz[3], double rgb[3]) {
	double uv[2];
	reverse_parametrize[object_type[object]](xyz, uv, object_parameters[object]);
	texture_map(object_texture[object], uv[_X], uv[_Y], rgb);
}

int trace_ray(
	double tail[3],
	double head[3],
	double rgb[3],
	int remaining_collisions,
	double intensity,
	int check_shadow
) {
	// Set to default black background
	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;

	if (remaining_collisions < 0) return 0;
	if (intensity < MINIMUM_INTENSITY) return 0;
	intensity *= INTENSITY_DECAY;

	// Keep track of closest object
	double t_min = -1;
	int closest_object = -1;
	double object_space_intersection[3] = {0, 0, 0};
	double intersection[3] = {0, 0, 0};
	double normal[3] = {0, 0, 0};

	for (int object = 0; object < objects; object++) {
		// Map input ray from world space to object space
		// Oject-space eye
		double E[3];
		M3d_mat_mult_pt(E, object_matrix_i[object], tail);
		// Object-space front of ray, where it intersects the screen
		double F[3];
		M3d_mat_mult_pt(F, object_matrix_i[object], head);

		// Compute vector along ray
		double D[3];
		for (int i = 0; i < 3; i++) {
			D[i] = F[i] - E[i];
		}

		// Find intersection point
		double t = solve_ray_intersection[object_type[object]](E, D, object_parameters[object]);

		// Move on if no closer intersection
		if (t <= EPSILON || (t_min > 0 && t > t_min)) continue;

		// Record closest intersection
		t_min = t;
		closest_object = object;

		// Compute intersection point
		for (int i = 0; i < 3; i++) {
			object_space_intersection[i] = E[i] + t * D[i];
		}

		// Find world-space normal vector
		double d[3];
		gradient[object_type[object]](object_space_intersection, d, object_parameters[object]);
		normal[0] = object_matrix_i[object][0][0] * d[0] + object_matrix_i[object][1][0] * d[1] + object_matrix_i[object][2][0] * d[2];
		normal[1] = object_matrix_i[object][0][1] * d[0] + object_matrix_i[object][1][1] * d[1] + object_matrix_i[object][2][1] * d[2];
		normal[2] = object_matrix_i[object][0][2] * d[0] + object_matrix_i[object][1][2] * d[1] + object_matrix_i[object][2][2] * d[2];

		// Map intersection point back to world space
		M3d_mat_mult_pt(intersection, object_matrix[object], object_space_intersection);
	}

	if (closest_object >= 0) {
		if (check_shadow) return t_min >= 0 && t_min <= 1;

		orient_normal(intersection, normal, tail);

		// Find reflected ray
		double r[3] = {0, 0, 0};
		double L[3];
		for (int i = 0; i < 3; i++) {
			L[i] = head[i] - tail[i];
		}
		find_reflection_vector(L, normal, r);
		normalize(r);
		double light_tail[3];
		double light_head[3];
		double reflected_tail[3];
		double reflected_head[3];
		double transmitted_tail[3];
		double transmitted_head[3];
		for (int i = 0; i < 3; i++) {
			double l = light_in_eye_space[i] - intersection[i];
			light_tail[i] = intersection[i] + l * EPSILON;
			light_head[i] = intersection[i] + l;

			reflected_tail[i] = intersection[i] + r[i] * EPSILON;
			reflected_head[i] = intersection[i] + r[i];

			double delta = head[i] - tail[i];
			transmitted_tail[i] = intersection[i] + delta * EPSILON;
			transmitted_head[i] = intersection[i] + delta;
		}

		// Find object casting a shadow
		double dummy_rgb[3];
		int shadowed = trace_ray(
			light_tail,
			light_head,
			dummy_rgb,
			0,
			MINIMUM_INTENSITY,
			1
		);

		// Find color from reflection
		double ref = object_reflectivity[closest_object];
		double reflected_rgb[3];
		trace_ray(
			reflected_tail,
			reflected_head,
			reflected_rgb,
			remaining_collisions - 1,
			intensity * ref,
			0
		);

		// Find transmitted color from behind object
		double trans = 1 - object_opacity[closest_object];
		double transmitted_rgb[3];
		trace_ray(
			transmitted_tail,
			transmitted_head,
			transmitted_rgb,
			remaining_collisions - 1,
			intensity * trans,
			0
		);

		// Find object color, including reflection and transparent transmission
		double inherent_rgb[3];
		if (object_texture[closest_object] == TM_SOLID_COLOR) {
			for (int i = 0; i < 3; i++) {
				inherent_rgb[i] = object_color[closest_object][i];
			}
		} else {
			shape_texture_map(closest_object, object_space_intersection, inherent_rgb);
		}
		double eye[3] = {0, 0, 0};
		Light_Model_rt(inherent_rgb, eye, intersection, normal, rgb, shadowed);
		for (int i = 0; i < 3; i++) {
			rgb[i] = rgb[i] * (1 - trans) + transmitted_rgb[i] * trans;
			rgb[i] = rgb[i] * (1 - ref) + reflected_rgb[i] * ref;
		}
		return 1;
	}
	return 0;
}

void ray(double tail[3], double head[3], double rgb[3]) {
	trace_ray(tail, head, rgb, MAXIMUM_REFLECTIONS, 1, 0);
}

void map_pixel_onto_world_space_screen(double p[3]) {
	p[_X] = (p[_X] * 2 - LG_WINDOW_WIDTH) * H / WINDOW_SIZE;
	p[_Y] = (p[_Y] * 2 - LG_WINDOW_HEIGHT) * H / WINDOW_SIZE;
	p[_Z] = 1;
}

void render() {
	double eye[3] = {0, 0, 0};
	for (int x = 0; x < LG_WINDOW_WIDTH; x++) {
		for (int y = 0; y < LG_WINDOW_HEIGHT; y++) {
			double p[3] = {x, y, 0};
			map_pixel_onto_world_space_screen(p);
			double rgb[3];
			ray(eye, p, rgb);
			LG_rgb(rgb[_R], rgb[_G], rgb[_B]);
			LG_point(x, y);
		}
	}
	LG_display_image();
}
