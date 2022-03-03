#include <fpt.h>
#include <m3d.h>

double obmat[100][4][4] ;
double obinv[100][4][4] ;
double color[100][3] ;
int    num_objects ;



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void normalize(double tail[3], double head[3]) {
	double d[3];
	double length = 0;
	for (int i = 0; i < 3; i++) {
		d[i] = head[i] - tail[i];
		length += d[i] * d[i];
	}
	length = sqrt(length);
	for (int i = 0; i < 3; i++) {
		head[i] = tail[i] + d[i] * 50 / length;
	}
}

int solve_quadratic(double a, double b, double c, double x[2]) {
	double root = b * b - 4 * a * c;
	if (root < 0) return 0;
	root = sqrt(root);
	x[0] = (-b - root) / (2 * a);
	if (root == 0) return 1;
	x[1] = (-b + root) / (2 * a);
	return 2;
}

double solve_quadratic_min_pos(double a, double b, double c) {
	double x[2] = {0, 0};
	int n = solve_quadratic(a, b, c, x);
	if (n == 0) return -1;
	if (n == 1) return x[0];
	if (x[0] < 0 && x[1] < 0) return -1;
	if (x[0] < 0) return x[1];
	if (x[1] < 0) return x[0];
	return fmin(x[0], x[1]);
}


double ray(double Rsource[3], double Rtip[3], double argb[3]) {
	argb[0] = 0;
	argb[1] = 0;
	argb[2] = 0;

	double t_min = -1;
	int obj = -1;
	double ixyz[3] = {0, 0, 0,};
	double Ntip[3] = {0, 0, 0};
	for (int ob = 0; ob < num_objects; ob++) {
		// World space ray source
		double wsrs[3];
		M3d_mat_mult_pt(wsrs, obinv[ob], Rsource);
		// World space ray tip
		double wsrt[3];
		M3d_mat_mult_pt(wsrt, obinv[ob], Rtip);
		double dxyz[3];
		for (int i = 0; i < 3; i++) {
			dxyz[i] = wsrt[i] - wsrs[i];
		}
		double a = dxyz[0] * dxyz[0] + dxyz[1] * dxyz[1];
		double b = 2 * (wsrs[0] * dxyz[0] + wsrs[1] * dxyz[1]);
		double c = wsrs[0] * wsrs[0] + wsrs[1] * wsrs[1] - 1;
		double t = solve_quadratic_min_pos(a, b, c);
		if (t < 0 || (t_min > 0 && t > t_min)) continue;
		for (int i = 0; i < 3; i++) {
			ixyz[i] = wsrs[i] + t * dxyz[i];
		}
		double theta = atan2(ixyz[1], ixyz[0]);
		Ntip[0] = ixyz[0] + cos(theta);
		Ntip[1] = ixyz[1] + sin(theta);
		Ntip[2] = ixyz[2];
		M3d_mat_mult_pt(ixyz, obmat[ob], ixyz);
		M3d_mat_mult_pt(Ntip, obmat[ob], Ntip);
		t_min = t;
		obj = ob;
	}
	if (t_min > 0) {
		normalize(ixyz, Ntip);
		G_rgb(0.5, 0.5, 0.5);
		G_line(Rsource[0], Rsource[1], ixyz[0], ixyz[1]);
		G_line(Ntip[0], Ntip[1], ixyz[0], ixyz[1]);
		G_rgb(color[obj][0], color[obj][1], color[obj][2]);
		G_fill_circle(Rtip[0], Rtip[1], 2);
		G_wait_key();
	}
}



void Draw_ellipsoid (int onum)
{
  int n,i ;
  double t, xyz[3] ;
  double x,y ;

  G_rgb (color[onum][0],color[onum][1],color[onum][2]) ;
  
  n = 1000 ;
  for (i = 0 ; i < n ; i++) {
    t = i*2*M_PI/n ;
    xyz[0] = cos(t) ;
    xyz[1] = sin(t) ;
    xyz[2] = 0 ;
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
    Draw_ellipsoid(onum) ;
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

    

    G_rgb(0,0,0) ;
    G_clear() ;

    Draw_the_scene() ;
    
    Rsource[0] =  20 ;  Rsource[1] =  400 ;  Rsource[2] = 0 ;    
    G_rgb(1,0,1) ; G_fill_circle(Rsource[0], Rsource[1], 3) ;
    G_rgb(1,0,1) ; G_line(100,200,  100,600) ;
    
    G_wait_key() ;
    
    double ytip ;
    for (ytip = 200 ; ytip <= 600 ; ytip++) {
      Rtip[0]    = 100 ;  Rtip[1]    = ytip ;  Rtip[2]   = 0  ;    

      G_rgb(1,1,0) ; G_line(Rsource[0],Rsource[1],  Rtip[0],Rtip[1]) ;
            ray (Rsource, Rtip, argb) ; 

      Draw_the_scene() ;
      G_wait_key() ;
    }

    G_rgb(1,1,1) ; G_draw_string("'q' to quit", 50,50) ;
    while (G_wait_key() != 'q') ;
    G_save_image_to_file("out/2d_Simple_RaytracerS.xwd") ;
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




int main()
{
  G_init_graphics(800,800);
  test01() ;
}
