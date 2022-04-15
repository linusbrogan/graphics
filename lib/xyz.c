class Object{
	int points;
	double pts[3 * points]
	Poly *polys[];
}
class Poly {
	int edges;
	int points[];
}


#include <stdio.h>

struct polygon {
	int points;
	double *point_list;
	int edges;
	int *polygon;
}

double *polygon_allocate_point_list(int n) {
	size_t size = sizeof(double) * 3 * n;
	void *p = malloc(size);
	if (p == NULL) {
		fprintf(stderr, "Could not allocate point list of %d doubles.\n", n);
		exit(1);
	}
	return p;
}

double *polygon_allocate_sequence(int n) {
	size_t size = sizeof(int) * n;
	void *p = malloc(size);
	if (p == NULL) {
		fprintf(stderr, "Could not allocate polygon list of %d ints.\n", n);
		exit(1);
	}
	return p;
}

struct polygon *polygon_allocate() {
	void *p = malloc(sizeof(struct polygon));
	if (p == NULL) {
		fprintf(stderr, "Could not allocate polygon list of %d ints.\n", n);
		exit(1);
	}
	return p;
}

struct polygon *polygon_load(char *file_name) {
	FILE* fp = fopen(file_name, "r");
	if (fp == NULL) {
		fprintf(stderr, "Could not read xyz file %s.\n", file_name);
		exit(1);
	}
	struct polygon *polygon = polygon_allocate();

	// Read points and find center of mass
	double xyz_avg[3] = {0, 0, 0};
	int pts;
	fscanf(fp, "%d", &(polygon->points));
	polygon->point_list = polygon_allocate_point_list(polygon->points);
	int pt;
	for (pt = 0; pt < polygon->points; pt++) {
		for (int i = 0; i < 3; i++) {
			double *coordinate = &(polygon->points[pt * 3 + i]);
			fscanf(fp, "%lf", coordinate);
			xyz_avg[i] += *coordinate;
		}
	}
	for (int i = 0; i < 3; i++) {
		xyz_avg[i] /= polygon->points;
	}
	for (pt = 0; pt <= polygon->points; pt++) {
		for (int i = 0; i < 3; i++) {
			double *coordinate = &(polygon->points[pt * 3 + i]);
			*coordinate -= xyz_avg[i];
		}
	}

	// Read polygons
	int polygons
	fscanf(fp, "%d", &polygons);
	for (int poly = 0; poly < polygons; poly++) {
		fscanf(file, "%d", &(polygon->edges));
		for (int pt = 0; pt < polygon->edges; pt++)
			fscanf(file, "%d", &polys[i][poly][pt]);
	}
}



//


	fclose(fp);
}

// Screen settings
#define X_MAX 600
#define X_MID ((X_MAX) / 2)
#define Y_MAX (X_MAX)
#define Y_MID ((Y_MAX) / 2)

// File constraints
#define MAX_PTS 500
#define MAX_POLYS 500
#define MAX_POLY_SIZE 20
#define MAX_FILES 7

// Field of view settings: half angle
#define h (M_PI / 4)
#define H (tan(h))

// Polygons
typedef struct {
	int item;
	int pgon;
} pgon_t;

// Number of shapes being drawn
int items = 0;
// Number of points
int numpts[MAX_FILES];
// x, y, z coordinates
double x[MAX_FILES][MAX_PTS];
double y[MAX_FILES][MAX_PTS];
double z[MAX_FILES][MAX_PTS];
// Number of polygons
int numpolys[MAX_FILES];
// Number of points in each polygon
int psize[MAX_FILES][MAX_POLYS];
// Sequence of points in each polygon
int polys[MAX_FILES][MAX_POLYS][MAX_POLY_SIZE];
// pgon_t structs for sorting polygons
pgon_t pgons[MAX_FILES * MAX_POLYS];

// Light settings
double LIGHT_X = 100;
double LIGHT_Y = 200;
double LIGHT_Z = (-50);
double I_AMBIENT = 0.2;
double I_DIFFUSE_MAX = 0.5;
double I_SPECULAR_POWER = 7;
double I_SPECULAR; // = (1 - ((I_AMBIENT) + (I_DIFFUSE_MAX)));
double HITHER = 0.1;
double YON = 100;

// Useful operations
double rot_small[4][4];
double rot_large[4][4];
double trans_up[4][4];
double trans_dn[4][4];

// Define colors for items
#define SPOKE_R 1
#define SPOKE_G 0
#define SPOKE_B 0
#define AXEL_R 0
#define AXEL_G 0
#define AXEL_B 1

/** Sorts pgon_t structs by distance (large to small). */
int compare_pgon(const void *p, const void *q) {
	pgon_t *a = (pgon_t *) p;
	pgon_t *b = (pgon_t *) q;

	double za = z[a->item][polys[a->item][a->pgon][0]];
	double zb = z[b->item][polys[b->item][b->pgon][0]];
	double diff = za - zb;

	if (diff > 0) return -1;
	if (diff < 0) return 1;
//	if (a->dist > b->dist) return -1;
//	if (a->dist < b->dist) return 1;
	return 0;
}

/** Reads a .xyz file into slot i. */
void readXYZ(FILE* file, int i) {
	double x_avg = 0;
	double y_avg = 0;
	double z_avg = 0;
	// Read points and find center of mass
	int pts;
	fscanf(file, "%d", &pts);
	numpts[i] = pts;

	int pt;
	for (pt = 0; pt < pts; pt++) {
		fscanf(file, "%lf %lf %lf", &x[i][pt], &y[i][pt], &z[i][pt]);
		x_avg += x[i][pt];
		y_avg += y[i][pt];
		z_avg += z[i][pt];
	}
	x[i][pts] = x_avg / pts;
	y[i][pts] = y_avg / pts;
	z[i][pts] = z_avg / pts;
	for (pt = 0; pt <= pts; pt++) {
		x[i][pt] -= x[i][pts];
		y[i][pt] -= y[i][pts];
		z[i][pt] -= z[i][pts];
	}

	// Read polygons
	fscanf(file, "%d", &numpolys[i]);
	for (int poly = 0; poly < numpolys[i]; poly++) {
		int pts;
		fscanf(file, "%d", &pts);
		psize[i][poly] = pts;
		for (int pt = 0; pt < pts; pt++)
			fscanf(file, "%d", &polys[i][poly][pt]);
	}
}

/** Projects object onto screen. */
void project(double * x_proj, double * y_proj, double x, double y, double z) {
	double scaleX = X_MID / H;
	double scaleY = Y_MID / H;
	double scale = fmin(scaleX, scaleY);
	*x_proj = scale * (x / z) + X_MID;
	*y_proj = scale * (y / z) + Y_MID;
}

/** Collects all polygons from all files as pgon_t structs in pgons. */
int collect_polygons() {
	int size = 0;
	for (int item = 0; item < items; item++) {
		for (int poly = 0; poly < numpolys[item]; poly++) {
			pgons[size].item = item;
			pgons[size].pgon = poly;
			int pt0 = polys[item][poly][0];
			//pgons[size].dist = z[item][pt0];
			size++;
		}
	}
	return size;
}

/** Returns the the dot product of two 3D vectors.*/
double vec_dot(double v[3], double w[3]) {
	double dot = 0;
	for (int i = 0; i < 3; i++)
		dot += v[i] * w[i];
	return dot;
}

/** Returns the magnitude of a 3D vector. */
double vec_magnitude(double v[3]) {
	return sqrt(vec_dot(v, v));
}

/** Safely computes into a the cross product of b and c. */
void vec_cross(double a[3], double b[3], double c[3]) {
	double res[3] = {0, 0, 0};
	res[0] = b[1] * c[2] - b[2] * c[1];
	res[1] = b[2] * c[0] - b[0] * c[2];
	res[2] = b[0] * c[1] - b[1] * c[0];
	for (int i = 0; i < 3; i++)
		a[i] = res[i];
}

/** Returns the intensity of a polygon. */
double intensity(pgon_t pgon) {
	double i = I_AMBIENT;
	int item = pgon.item;
	int poly = pgon.pgon;
	if (psize[item][poly] < 3) return i;
	int pt0 = polys[item][poly][0];
	int pt1 = polys[item][poly][1];
	int pt2 = polys[item][poly][2];

	// Find normal unit vector by crossing two polygon-spanning vectors
	double span1[3] = {
		x[item][pt1] - x[item][pt0],
		y[item][pt1] - y[item][pt0],
		z[item][pt1] - z[item][pt0]
	};
	double span2[3] = {
		x[item][pt2] - x[item][pt0],
		y[item][pt2] - y[item][pt0],
		z[item][pt2] - z[item][pt0]
	};

	double n_hat[3];
	vec_cross(n_hat, span1, span2);
	double mag = vec_magnitude(n_hat);
	for (int i = 0; i < 3; i++)
		n_hat[i] /= mag;

	// Find unit vectors to eye and to light
	double e_hat[3] = {
		0 - x[item][pt0],
		0 - y[item][pt0],
		0 - z[item][pt0]
	};
	mag = vec_magnitude(e_hat);
	for (int i = 0; i < 3; i++)
		e_hat[i] /= mag;
	double l_hat[3] = {
		LIGHT_X - x[item][pt0],
		LIGHT_Y - y[item][pt0],
		LIGHT_Z - z[item][pt0]
	};
	mag = vec_magnitude(l_hat);
	for (int i = 0; i < 3; i++)
		l_hat[i] /= mag;

	double cos_alpha = vec_dot(n_hat, l_hat);

	// Find reflection vector
	double r_hat[3];
	for (int i = 0; i < 3; i++)
		r_hat[i] = 2 * cos_alpha * n_hat[i] - l_hat[i];

	double cos_beta = vec_dot(e_hat, r_hat);

	// If eye sees light, add diffuse and specular intensity
	if (vec_dot(n_hat, e_hat) * vec_dot(n_hat, l_hat) >= 0) {
		i += I_DIFFUSE_MAX * fabs(cos_alpha);
		i += I_SPECULAR * pow(cos_beta, I_SPECULAR_POWER);
	}

	return i;
}

/** Sets color for a polygon. */
void polygon_color(pgon_t pgon) {
	int file = pgon.item;
	double red = SPOKE_R;
	double green = SPOKE_G;
	double blue = SPOKE_B;
	if (file == 0) {
		red = AXEL_R;
		green = AXEL_G;
		blue = AXEL_B;
	}
	double i = intensity(pgon);
	double threshold = I_AMBIENT + I_DIFFUSE_MAX;

	if (i < threshold) {
		double scale = i / threshold;
		red *= scale;
		green *= scale;
		blue *= scale;
	}

	if (i > threshold) {
		double scale = (i - threshold) / (1 - threshold);
		red += scale * (1 - red);
		green += scale * (1 - green);
		blue += scale * (1 - blue);
	}

	G_rgb(red, green, blue);
}

/** Saves into (*x_intersection, *y_intersection, *z_intersection) the intersection point of plane(a, b, c, d) and line(1, 2). */
void save_intersection(
	// Save into
	double* x_intersection,
	double* y_intersection,
	double* z_intersection,
	// Plane
	double a,
	double b,
	double c,
	double d,
	// Line
	double x1,
	double y1,
	double z1,
	double x2,
	double y2,
	double z2
) {
	double t = -(a * x1 + b * y1 + c * z1 + d) / (a * (x2 - x1) + b * (y2 - y1) + c * (z2 - z1));
	*x_intersection = x1 + t * (x2 - x1);
	*y_intersection = y1 + t * (y2 - y1);
	*z_intersection = z1 + t * (z2 - z1);
}

/** Clips a polygon in place. */
int clip_polygon_against_plane(
	double xc[],
	double yc[],
	double zc[],
	double xp[],
	double yp[],
	double zp[],
	int size,
	double abcd[]
) {
	double X[MAX_POLY_SIZE];
	double Y[MAX_POLY_SIZE];
	double Z[MAX_POLY_SIZE];

	double z_mid = (HITHER + YON) / 2;

	// For each line of the clipping polygon, clip the polygon
	// Find parameters for clipping line
	double a = abcd[0];
	double b = abcd[1];
	double c = abcd[2];
	double d = abcd[3];

	// Find inside
	double sign_inside = a * 0 + b * 0 + c * z_mid + d;

	int size_cut = 0;
	double sign_point = a * xp[0] + b * yp[0] + c * zp[0] + d;

	// Check each point, saving good points and intersections
	for (int j = 0; j < size; j++) {
		int next = (j + 1) % size;
		double sign_point_next = a * xp[next] + b * yp[next] + c * zp[next] + d;

		// If crossing the clipping line, save intersection
		if (sign_point * sign_point_next < 0) {
			save_intersection(&X[size_cut], &Y[size_cut], &Z[size_cut], a, b, c, d, xp[j], yp[j], zp[j], xp[next], yp[next], zp[next]);
			size_cut++;
		}

		// If next is inside, save it
		if (sign_inside * sign_point_next >= 0) {
			X[size_cut] = xp[next];
			Y[size_cut] = yp[next];
			Z[size_cut] = zp[next];
			size_cut++;
		}

		sign_point = sign_point_next;
	}

	size = size_cut;
	for (int j = 0; j < size; j++) {
		xc[j] = X[j];
		yc[j] = Y[j];
		zc[j] = Z[j];
	}

	return size;
}

/* Clips p into c. */
int clip_polygon(double xc[], double yc[], double zc[], double xp[], double yp[], double zp[], int size) {
	double abcd[6 * 4] = {
		0, 0, 1, -HITHER, // front
		0, 0, 1, -YON, // back
		0, 1, -H, 0, // top
		0, 1, H, 0, // bottom
		1, 0, -H, 0, //left
		1, 0, H, 0 // right
	};
	double X[MAX_POLY_SIZE];
	double Y[MAX_POLY_SIZE];
	double Z[MAX_POLY_SIZE];
	for (int i = 0; i < size; i++) {
		X[i] = xp[i];
		Y[i] = yp[i];
		Z[i] = zp[i];
	}
	for (int i = 0; i < 6 * 4; i += 4) {
		size = clip_polygon_against_plane(X, Y, Z, X, Y, Z, size, &abcd[i]);
	}
	for (int i = 0; i < size; i++) {
		xc[i] = X[i];
		yc[i] = Y[i];
		zc[i] = Z[i];
	}
	return size;
}

/** Draw the files. */
void drawXYZ() {
	int npolys = collect_polygons();
	qsort(pgons, npolys, sizeof(pgon_t), compare_pgon);
	for (int p = 0; p < npolys; p++) {
		// Copy points into polygon
		int poly = pgons[p].pgon;
		int file = pgons[p].item;
		int size = psize[file][poly];
		double xp[size];
		double yp[size];
		double zp[size];
		for (int i = 0; i < size; i++) {
			int pt = polys[file][poly][i];
			xp[i] = x[file][pt];
			yp[i] = y[file][pt];
			zp[i] = z[file][pt];
		}

		// Clip by Field of View
		double xc[MAX_POLY_SIZE];
		double yc[MAX_POLY_SIZE];
		double zc[MAX_POLY_SIZE];
		size = clip_polygon(xc, yc, zc, xp, yp, zp, size);

		// Project onto screen
		double xs[MAX_POLY_SIZE];
		double ys[MAX_POLY_SIZE];
		for (int i = 0; i < size; i++)
			project(&xs[i], &ys[i], xc[i], yc[i], zc[i]);
		polygon_color(pgons[p]);
		G_fill_polygon(xs, ys, size);
		G_rgb(0, 0, 0);
//		G_polygon(xs, ys, size);
	}
}

/** Runs a matrix operation. */
void apply(int item, double operation[4][4]) {
	int pt = numpts[item];
	M3d_mat_mult_points(x[item], y[item], z[item], operation, x[item], y[item], z[item], pt + 1);
}

/** Handles key press actions. */
void handleInput() {
	char c = '0';
	int item = 0;
	while (c != 'q') {
		if (c == 'r') {
			for (int item = 0; item < MAX_FILES; item++) {
				int pt = numpts[item];
				// Translate to origin
				double translation[4][4];
				M3d_make_translation(translation, -x[item][pt], -y[item][pt], -z[item][pt]);
				// Rotate
				double operation[4][4];
				M3d_mat_mult(operation, rot_small, translation);
				// Translate back
				M3d_make_translation(translation, x[item][pt], y[item][pt], z[item][pt]);
				M3d_mat_mult(operation, translation, operation);
				apply(item, operation);
			}
		}

		// Render
		G_rgb(0, 0, 0);
		G_clear();
		drawXYZ();

		c = G_wait_key();
	}
}

/** Puts the shapes in the right places. */
void setup() {
	double dtheta = M_PI / 3;

	// Initialize common operations
	M3d_make_translation(trans_up, 0, 1.5, 3);
	M3d_make_translation(trans_dn, 0, -1.5, 3);
	M3d_make_y_rotation_cs(rot_small, cos(M_PI / 60), sin(M_PI / 60));
	M3d_make_y_rotation_cs(rot_large, cos(dtheta), sin(dtheta));

	// Position the axel
	double axel_op[4][4];
	M3d_make_scaling(axel_op, 1.5, 1, 1);
	double trans_back[4][4];
	M3d_make_translation(trans_back, 0, 0, 3);
	M3d_mat_mult(axel_op, trans_back, axel_op);
	double rot_vertical[4][4];
	M3d_make_z_rotation_cs(rot_vertical, cos(M_PI / 2), sin(M_PI / 2));
	M3d_mat_mult(axel_op, rot_vertical, axel_op);
	apply(0, axel_op);

	// Position the spokes
	double rot[4][4];
	M3d_make_identity(rot);
	double op[4][4];
	int item = 1;
	while (item < 4) {
		M3d_mat_mult(op, trans_up, rot);
		apply(item, op);
		M3d_mat_mult(rot, rot, rot_large);
		item++;
	}
	while (item < 7) {
		M3d_mat_mult(op, trans_dn, rot);
		apply(item, op);
		M3d_mat_mult(rot, rot, rot_large);
		item++;
	}
}

	// Load the cylinder
	FILE* fp;
	for (int i = 0; i < MAX_FILES; i++) {
		items++;
		fp = fopen("cylinder.xyz", "r");
		if (fp == NULL) {
			perror("lbf: cannot access file");
			return 1;
		}

		readXYZ(fp, i);
		fclose(fp);
	}

	setup();
	handleInput();
}
