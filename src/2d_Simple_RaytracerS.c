#include <fpt.h>
#include <m3d.h>
#include <shape_2d.h>

#define MAXIMUM_REFLECTIONS 4

double obmat[100][4][4] ;
double obinv[100][4][4] ;
double color[100][3] ;
int    num_objects ;



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
enum object_type {
	OBJ_CIRCLE = 0,
	OBJ_HALF_HYPERBOLA,
	OBJ_HYPERBOLA,
	OBJ_LINE
};

int (*shape_xyz[4])(int, int, double[3]) = {
	circle_xyz,
	half_hyperbola_xyz,
	hyperbola_xyz,
	line_xyz
};

void (*gradient[4])(double, double, double[2]) = {
	d_circle,
	d_hyperbola,
	d_hyperbola,
	d_line
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
		double d[2];
		gradient[object_types[object]](intersection[0], intersection[1], d);
		normal[0] = obinv[object][0][0] * d[0] + obinv[object][1][0] * d[1];
		normal[1] = obinv[object][0][1] * d[0] + obinv[object][1][1] * d[1];

		// Map intersection point back to world space
		M3d_mat_mult_pt(intersection, obmat[object], intersection);
	}

	if (closest_object >= 0) {
		// Find head of normal vector from intersection point
		double normal_head[3];
		find_normal_head(intersection, normal, tail, normal_head);

		// Save color at intersection
		for (int i = 0; i < 3; i++) {
			rgb[i] = color[closest_object][i];
		}

		// Draw intersection point and ray to intersection
		G_rgb(rgb[0], rgb[1], rgb[2]);
		G_fill_circle(intersection[0], intersection[1], 2);
		G_line(tail[0], tail[1], intersection[0], intersection[1]);

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
		G_rgb(1, 1, 1);
		G_line(intersection[0], intersection[1], R[0], R[1]);
		double reflected_rgb[3];
		reflect_ray(intersection, R, reflected_rgb, remaining_reflections - 1);
	}
}

void ray(double tail[3], double head[3], double rgb[3]) {
	reflect_ray(tail, head, rgb, MAXIMUM_REFLECTIONS);
}


void Draw_shape(int onum) {
  int n,i ;
  double t, xyz[3] ;
  double x,y ;

  G_rgb (color[onum][0],color[onum][1],color[onum][2]) ;
  
  n = 1000 ;
  for (i = 0 ; i < n ; i++) {
		int draw = shape_xyz[object_types[onum]](i, n, xyz);
		if (!draw) continue;
    M3d_mat_mult_pt(xyz, obmat[onum], xyz) ;
    x = xyz[0] ;
    y = xyz[1] ;
    G_point(x,y) ;
  }

}




void Draw_the_scene()
{
  int onum ;
  for (onum = 0 ; onum < num_objects ; onum++) {
		Draw_shape(onum);
  }
}


void project_head_onto_screen(double tail[3], double head[3]) {
	double d[3];
	for (int i = 0; i < 3; i++) {
		d[i] = head[i] - tail[i];
	}
	double scale = (100 - tail[0]) / d[0];
	for (int i = 0; i < 3; i++) {
		d[i] *= scale;
		head[i] = tail[i] + d[i];
	}
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////





int test01()
{
  double vm[4][4], vi[4][4];
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
	object_types[num_objects] = OBJ_CIRCLE;
    color[num_objects][0] = 0.0 ;
    color[num_objects][1] = 0.8 ; 
    color[num_objects][2] = 0.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =   60   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =  100   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =   25   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  300   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  200   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this

    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_CIRCLE;
    color[num_objects][0] = 1.0 ;
    color[num_objects][1] = 0.3 ; 
    color[num_objects][2] = 0.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  180   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =   40   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =   60   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  400   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  550   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_CIRCLE;
    color[num_objects][0] = 0.3 ;
    color[num_objects][1] = 0.3 ; 
    color[num_objects][2] = 1.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =   75   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =   35   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =  150   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  360   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  500   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this        
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_CIRCLE;
    color[num_objects][0] = 0.5 ;
    color[num_objects][1] = 1.0 ; 
    color[num_objects][2] = 1.0 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  130   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =   30   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =  -15   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  100   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  700   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this        
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_HYPERBOLA;
    color[num_objects][0] = 0.8;
    color[num_objects][1] = 0;
    color[num_objects][2] = 0.2;

    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  15   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =   80   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =  -7   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  200   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  630   ; Tn++ ;

    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this        
    //////////////////////////////////////////////////////////////
	object_types[num_objects] = OBJ_LINE;
    color[num_objects][0] = 0.8;
    color[num_objects][1] = 0;
    color[num_objects][2] = 0.2;

    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  50   ; Tn++ ;
    Ttypelist[Tn] = RZ ; Tvlist[Tn] =  110   ; Tn++ ;
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  300   ; Tn++ ;
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  300   ; Tn++ ;

    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this        
    //////////////////////////////////////////////////////////////

    

	int first = 1;

	while(1) {
	double p[2] = {0, 0};
	if (!first) {
		G_wait_click(p);
		if (p[0] < 80 && p[1] < 15) exit(0);
	}

    G_rgb(0,0,0) ;
    G_clear() ;

    Draw_the_scene() ;
    
    Rsource[0] =  20 ;  Rsource[1] =  400 ;  Rsource[2] = 0 ;    
    G_rgb(1,0,1) ; G_fill_circle(Rsource[0], Rsource[1], 3) ;
    G_rgb(1,0,1) ; G_line(100,200,  100,600) ;
    
	G_rgb(1, 1, 1);
	G_fill_rectangle(0, 0, 80, 15);
	G_rgb(0, 0, 0);
	G_draw_string("Click to quit", 0, 0);

	
	
	if (first) {
		first = 0;
		continue;
	}
	Rtip[0] = p[0];
	Rtip[1] = p[1];
	Rtip[2] = 0;
	project_head_onto_screen(Rsource, Rtip);
      G_rgb(1,1,0) ; G_line(Rsource[0],Rsource[1],  Rtip[0],Rtip[1]) ;
            ray (Rsource, Rtip, argb) ; 

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
