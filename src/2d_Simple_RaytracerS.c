#include <fpt.h>
#include <m3d.h>
#include <shape_2d.h>
#include <rt_utils.h>

#define MAXIMUM_REFLECTIONS 4

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
double object_refractive_index[100];

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
		int normal_sign = orient_normal(intersection, normal, tail);

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
		}

		// Find refracted ray
		double refracted[3] = {0, 0, 0};
		double n_obj = object_refractive_index[closest_object];
		double n1 = 1; // Vacuum vacuum outside of object (might want to be more clever)
		double n2 = n_obj;
		// Check ray direction: inward or outward?
		double refraction_orientation = normal_sign * dot_product(L, normal);
		if (refraction_orientation > 0) {
			double temp = n1;
			n1 = n2;
			n2 = temp;
		}
		int refraction = find_refraction_vector(n1, n2, normal, L, refracted);

		double refracted_rgb[3] = {0, 0, 0};
		if (refraction) {
			normalize(refracted);
			double refracted_tail[3];
			double refracted_head[3];
			double normal_head[3];
			double normal_tail[3];
			for (int i = 0; i < 3; i++) {
				refracted_tail[i] = intersection[i] + refracted[i] * 1e-5;
				refracted_head[i] = intersection[i] + refracted[i] * 30;
				normal_head[i] = intersection[i] + normal[i] * 30;
				normal_tail[i] = intersection[i] - normal[i] * 30;
			}

			// Indicate normal and direction of refraction
			G_rgb(0.75, 0.5, 0.5);
			G_line(intersection[0], intersection[1], normal_head[0], normal_head[1]);
			G_rgb(0.5, 0.75, 0.75);
			G_line(intersection[0], intersection[1], normal_tail[0], normal_tail[1]);
			G_rgb(1, 1, 1);
			G_line(refracted_tail[0], refracted_tail[1], refracted_head[0], refracted_head[1]);

			// Find refracted color
			reflect_ray(refracted_tail, refracted_head, refracted_rgb, remaining_reflections - 1);
		}

		// Find object color, including refraction
		double refr = 0.5;
		for (int i = 0; i < 3; i++) {
			rgb[i] = color[closest_object][i] * (1 - refr) + refracted_rgb[i] * refr;
		}

		// Draw intersection point and ray to intersection
		G_rgb(rgb[0], rgb[1], rgb[2]);
		G_fill_circle(intersection[0], intersection[1], 2);
		G_line(tail[0], tail[1], intersection[0], intersection[1]);
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
	object_refractive_index[num_objects] = 1.5;
    color[num_objects][0] = 0.0 ;
    color[num_objects][1] = 0.8 ; 
    color[num_objects][2] = 0.0 ;
	reflectivity[num_objects] = 0;
	
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
	object_refractive_index[num_objects] = 1;
    color[num_objects][0] = 1.0 ;
    color[num_objects][1] = 0.3 ; 
    color[num_objects][2] = 0.0 ;
	reflectivity[num_objects] = 0;
	
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
	object_refractive_index[num_objects] = 1;
    color[num_objects][0] = 0.3 ;
    color[num_objects][1] = 0.3 ; 
    color[num_objects][2] = 1.0 ;
	reflectivity[num_objects] = 0;
	
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
	object_refractive_index[num_objects] = 1;
    color[num_objects][0] = 0.5 ;
    color[num_objects][1] = 1.0 ; 
    color[num_objects][2] = 1.0 ;
	reflectivity[num_objects] = 0;
	
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
	object_refractive_index[num_objects] = 3;
    color[num_objects][0] = 0.8;
    color[num_objects][1] = 0;
    color[num_objects][2] = 0.2;
	reflectivity[num_objects] = 0.5;

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
	object_refractive_index[num_objects] = 1.33;
    color[num_objects][0] = 0.8;
    color[num_objects][1] = 0;
    color[num_objects][2] = 0.2;
	reflectivity[num_objects] = 0.9;

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
	int exit_box_width = 80;
	int exit_box_height = 15;
	while (1) {
		double p[2] = {0, 0};
		if (!first) {
			G_wait_click(p);
			if (p[0] < exit_box_width && p[1] < exit_box_height) exit(0);
		}

    G_rgb(0,0,0) ;
    G_clear() ;

    Draw_the_scene() ;
    
    Rsource[0] =  20 ;  Rsource[1] =  400 ;  Rsource[2] = 0 ;    
    G_rgb(1,0,1) ; G_fill_circle(Rsource[0], Rsource[1], 3) ;
    G_rgb(1,0,1) ; G_line(100,200,  100,600) ;
    
		G_rgb(1, 1, 1);
		G_fill_rectangle(0, 0, exit_box_width, exit_box_height);
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
