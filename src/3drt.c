#include <fpt.h>
#include <m3d.h>
#include <shape_2d.h>
#include <lg.h>
#include <light_model.h>

#define MAXIMUM_REFLECTIONS 4
#define h (M_PI / 6)
#define H tan(h)



double obmat[100][4][4] ;
double obinv[100][4][4] ;
double color[100][3] ;
double reflectivity[100];
int    num_objects ;



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
enum object_type {
	OBJ_CIRCLE = 0,
	OBJ_HALF_HYPERBOLA,
	OBJ_HYPERBOLA,
	OBJ_LINE,
	OBJ_SPHERE,
	OBJ_PLANE
};

int (*shape_xyz[6])(int, int, double[3]) = {
	circle_xyz,
	half_hyperbola_xyz,
	hyperbola_xyz,
	line_xyz,
	circle_xyz
};

void d_sphere(double p[3], double d[3]) {
	d[0] = 2 * p[0];
	d[1] = 2 * p[1];
	d[2] = 2 * p[2];
}

void d_plane(double p[3], double d[3]) {
	d[0] = 0;
	d[1] = 0;
	d[2] = 1;
}

void (*gradient[6])(double[3], double[3]) = {
	NULL, NULL, NULL, NULL,
	d_sphere,d_plane
};

enum object_type object_types[100];

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

void find_normal_head(double intersection[3], double normal[3], double eye[3], double normal_head[3]) {
	// Find vector from intersection point to eye
	double e[3];
	for (int i = 0; i < 3; i++) {
		e[i] = eye[i] - intersection[i];
	}

	// Orient the normal on the same side as the eye
	double dot = dot_product(normal, e);
	if (dot < 0) {
		for (int i = 0; i < 3; i++) {
			normal[i] *= -1;
		}
	}

	normalize(normal);

	// Compute the head of the normal vector, scaled nicely
	for (int i = 0; i < 3; i++) {
		normal_head[i] = intersection[i] + normal[i] * 50;
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

double solve_ray_intersection(int object, double E[3], double D[3]) {
	switch (object_types[object]) {
		case OBJ_SPHERE: {
			double a = sq(D[0]) + sq(D[1]) + sq(D[2]);
			double b = 2 * (E[0] * D[0] + E[1] * D[1] + E[2] * D[2]);
			double c = sq(E[0]) + sq(E[1]) + sq(E[2]) - 1;

			double t[2] = {-1, -1};
			int n = solve_quadratic(a, b, c, t);

			if (n == 0) return -1;
			if (n == 1) return t[0];
			if (t[0] <= 0 && t[1] <= 0) return -1;
			if (t[0] <= 0) return t[1];
			if (t[1] <= 0) return t[0];
			return fmin(t[0], t[1]);
		}

		case OBJ_CIRCLE: {
			double a = sq(D[0]) + sq(D[1]);
			double b = 2 * (E[0] * D[0] + E[1] * D[1]);
			double c = sq(E[0]) + sq(E[1]) - 1;

			double t[2] = {-1, -1};
			int n = solve_quadratic(a, b, c, t);

			if (n == 0) return -1;
			if (n == 1) return t[0];
			if (t[0] <= 0 && t[1] <= 0) return -1;
			if (t[0] <= 0) return t[1];
			if (t[1] <= 0) return t[0];
			return fmin(t[0], t[1]);
		}

		case OBJ_HALF_HYPERBOLA: {
			double a = sq(D[0]) - sq(D[1]);
			double b = 2 * (E[0] * D[0] - E[1] * D[1]);
			double c = sq(E[0]) - sq(E[1]) - 1;

			double t[2] = {-1, -1};
			int n = solve_quadratic(a, b, c, t);

			if (n == 0) return -1;

			// Check for invalid solutions
			for (int solution = 0; solution < 2; solution++) {
				double x = E[0] + t[solution] * D[0];
				double y = E[1] + t[solution] * D[1];
				if (fabs(y) > 1 || x < 0 || t[solution] <= 0) {
					t[solution] = -1;
				}
			};

			if (n == 1) return t[0];
			if (t[0] <= 0 && t[1] <= 0) return -1;
			if (t[0] <= 0) return t[1];
			if (t[1] <= 0) return t[0];
			return fmin(t[0], t[1]);
		}

		case OBJ_HYPERBOLA: {
			double a = sq(D[0]) - sq(D[1]);
			double b = 2 * (E[0] * D[0] - E[1] * D[1]);
			double c = sq(E[0]) - sq(E[1]) - 1;

			double t[2] = {-1, -1};
			int n = solve_quadratic(a, b, c, t);

			if (n == 0) return -1;

			// Check for invalid solutions
			for (int solution = 0; solution < 2; solution++) {
				double y = E[1] + t[solution] * D[1];
				if (fabs(y) > 1 || t[solution] <= 0) {
					t[solution] = -1;
				}
			};

			if (n == 1) return t[0];
			if (t[0] <= 0 && t[1] <= 0) return -1;
			if (t[0] <= 0) return t[1];
			if (t[1] <= 0) return t[0];
			return fmin(t[0], t[1]);
		}

		case OBJ_PLANE: {
			if (D[2] == 0) return -1;
			double t = -E[2] / D[2];
			double x = E[0] + t * D[0];
			double y = E[1] + t * D[1];
			if (fabs(x) > 1 || fabs(y) > 1) return -1;
			return t;
		}

		case OBJ_LINE: {
			if (D[1] == 0) return -1;
			double t = -E[1] / D[1];
			if (fabs(E[0] + t * D[0]) > 1) return -1;
			return t;
		}
	}
}

void reflect_ray(double tail[3], double head[3], double rgb[3], int remaining_reflections) {
	// Set to default black background
	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;

	if (remaining_reflections < 0) return;

	// Keep track of closest object
	double t_min = -1;
	int closest_object = -1;
	double intersection[3] = {0, 0, 0};
	double normal[3] = {0, 0, 0};

	for (int object = 0; object < num_objects; object++) {
		// Map input ray from world space to object space
		// Oject-space eye
		double E[3];
		M3d_mat_mult_pt(E, obinv[object], tail);
		// Object-space front of ray, where it intersects the screen
		double F[3];
		M3d_mat_mult_pt(F, obinv[object], head);

		// Compute vector along ray
		double D[3];
		for (int i = 0; i < 3; i++) {
			D[i] = F[i] - E[i];
		}

		// Find intersection point
		double t = solve_ray_intersection(object, E, D);

		// Move on if no closer intersection
		if (t <= 1e-5 || (t_min > 0 && t > t_min)) continue;

		// Record closest intersection
		t_min = t;
		closest_object = object;

		// Compute intersection point
		for (int i = 0; i < 3; i++) {
			intersection[i] = E[i] + t * D[i];
		}

		// Find world-space normal vector
		double d[3];
		gradient[object_types[object]](intersection, d);
		normal[0] = obinv[object][0][0] * d[0] + obinv[object][1][0] * d[1] + obinv[object][2][0] * d[2];
		normal[1] = obinv[object][0][1] * d[0] + obinv[object][1][1] * d[1] + obinv[object][2][1] * d[2];
		normal[2] = obinv[object][0][2] * d[0] + obinv[object][1][2] * d[1] + obinv[object][2][2] * d[2];

		// Map intersection point back to world space
		M3d_mat_mult_pt(intersection, obmat[object], intersection);
	}

	if (closest_object >= 0) {
		// Find head of normal vector from intersection point
		double normal_head[3];
		find_normal_head(intersection, normal, tail, normal_head);

		// Save color at intersection
		double irgb[3];
		for (int i = 0; i < 3; i++) {
			irgb[i] = color[closest_object][i];
		}
		double eye[3] = {0, 0, 0};
		Light_Model(irgb, eye, intersection, normal, rgb);


		// Draw intersection point and ray to intersection
		G_rgb(rgb[0], rgb[1], rgb[2]);

		// Find reflected ray
		double r[3] = {0, 0, 0};
		double L[3];
		for (int i = 0; i < 3; i++) {
			L[i] = head[i] - tail[i];
		}
		find_reflection_vector(L, normal, r);
		normalize(r);
		double R[3];
		for (int i = 0; i < 3; i++) {
			R[i] = r[i] * 30 + intersection[i];
			intersection[i] += r[i] * 1e-10;
		}
		double reflected_rgb[3];
		reflect_ray(intersection, R, reflected_rgb, remaining_reflections - 1);
		for (int i = 0; i < 3; i++) {
			double ref = reflectivity[closest_object];
			rgb[i] = rgb[i] * (1 -  ref) + ref * reflected_rgb[i];
		}
	}
}

void ray(double tail[3], double head[3], double rgb[3]) {
	reflect_ray(tail, head, rgb, MAXIMUM_REFLECTIONS);
}

void proj_px_to_screen(double x, double y, double px[3]) {
	px[0] = (x - 800 / 2) * H / 400;
	px[1] = (y - 800 / 2) * H / 400;
	px[2] = 1;
}

int test01()
{
for (int frame=0;;frame++){  double vm[4][4], vi[4][4];
  double Tvlist[100];
  int Tn, Ttypelist[100];
  double m[4][4], mi[4][4];
  double Rsource[3];
  double Rtip[3];
  double argb[3] ;

    //////////////////////////////////////////////////////////////////////
    M3d_make_identity(vm) ;    M3d_make_identity(vi) ; // OVERRIDE for 2d
    //////////////////////////////////////////////////////////////////////

    num_objects = 0 ;

    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_SPHERE;
	reflectivity[num_objects] = 0.5;
    color[num_objects][0] = 0.0 ;
    color[num_objects][1] = 0.8 ; 
    color[num_objects][2] = 0.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = TZ ; Tvlist[Tn] =  10   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  3   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  3 * sin(frame * M_PI / 50)   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this

    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_SPHERE;
	reflectivity[num_objects] = 0;
    color[num_objects][0] = 0.8 ;
    color[num_objects][1] = 0.0 ; 
    color[num_objects][2] = 0.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  1.2   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =  2   ; Tn++ ;
    Ttypelist[Tn] = TZ ; Tvlist[Tn] =  10   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  -2   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  2 * sin(M_PI / 4 + frame * M_PI / 75)   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_PLANE;
	reflectivity[num_objects] = 0.8;
    color[num_objects][0] = 0.0 ;
    color[num_objects][1] = 0.3 ; 
    color[num_objects][2] = 0.5 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  5   ; Tn++ ;
    Ttypelist[Tn] = RX ; Tvlist[Tn] =  30   ; Tn++ ;
    Ttypelist[Tn] = RX ; Tvlist[Tn] =  10   ; Tn++ ;
    Ttypelist[Tn] = TZ ; Tvlist[Tn] =  10   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this
    //////////////////////////////////////////////////////////////

    double width = 800,height=800;
	double eye[3] = {0, 0, 0},px[3];
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++) {
			proj_px_to_screen(x, y, px);
            ray (eye, px, argb) ; 
			G_rgb(argb[0], argb[1], argb[2]);
			G_point(x, y);
		}
	}
	G_wait_key();
}
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




int main()
{
  G_init_graphics(800,800);
  test01() ;
}
