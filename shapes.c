#include "FPToolkit.c"
#include "M3d_matrix_tools.c"
#include "2d_shape_functions.c"

#define NUM_PTS 1000
#define NUM_SHAPES 9
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH)

void plot_shape(double (*fx)(double), double (*fy)(double), double low, double high, double *x, double *y, int points) {
	double dt = (high - low) / (points - 1 + 1); // TODO -1 is better
	double u = low;
	for (int i = 0; i < points; i++) {
		x[i] = fx(u);
		y[i] = fy(u);
		u += dt;
	}
}

void graph(double *x, double *y, int points) {
	for (int i = 0; i < points; i++) {
		G_point(x[i], y[i]);
	}
}

int main() {
	G_init_graphics(WINDOW_WIDTH, WINDOW_HEIGHT);
	G_rgb(0, 0, 0);
	G_clear();

	double x[NUM_SHAPES][NUM_PTS];
	double y[NUM_SHAPES][NUM_PTS];
	double z[NUM_SHAPES][NUM_PTS];

	int Tn = 0;
	int Ttypelist[100];
	double Tvlist[100];
	double A[4][4];
	double A_i[4][4];

	int shape = 0;

	G_rgb(1, 0, 0);
	plot_shape(cos, sin, 0.25 * M_PI, 1.5 * M_PI, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// circle :
//   x^2 + y^2 = 1
//
//   u in [0,2*M_PI] gives the entire circle :
//   x = cos(u) ;  y = sin(u) ; 
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =   50.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  100.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  300.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  500.0 ; Tn++ ;
// Graph only the part with u in [0.25*M_PI, 1.5*M_PI]
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;

	G_rgb(0, 1, 0);
	plot_shape(sum4_x, sum4_y, -1, 1, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sum4 :
//   x^4 + y^4 = 1
//
//   u in [-1,1] gives the upper branch of the curve :
//   x = u  ;  y = pow(1 - u*u*u*u, 0.25) ; 
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =   30.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   30.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  250.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  170.0 ; Tn++ ;
// Graph the entire curve.
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(0, 0, 1);
	plot_shape(square_x, square_y, 0, 4, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// square :
//   |x| + |y| = 1
//
//   u in [0,4] gives the entire square :
//     u in [0,1],  w = u     ;  x = 1 - w ;  y = w     ; 
//     u in [1,2],  w = u - 1 ;  x = -w    ;  y = 1 - w ;    
//     u in [2,3],  w = u - 2 ;  x = w - 1 ;  y = -w    ;
//     u in [3,4],  w = u - 3 ;  x = w     ;  y = w - 1 ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  150.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   70.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  500.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  460.0 ; Tn++ ;
// Graph the entire curve.
  	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(0.75, 0.5, 1);
	plot_shape(square2_x, square2_y, 0, 2 * M_PI, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// square (alternate parameterization) :
//   |x| + |y| = 1
//
//   u in [0,2*M_PI] gives the entire square :
//   x = sgn(cos(u))*[cos(u)]^2 ;   y = sgn(sin(u))*[sin(u)]^2  ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  150.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   70.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  500.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  670.0 ; Tn++ ;
// Graph the entire curve.
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(1, 1, 0);
	plot_shape(astroid_x, astroid_y, 0, 2 * M_PI, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// astroid :
//   sqrt(|x|) + sqrt(|y|) = 1
//
//   u in [0,2*M_PI] gives the entire astroid :
//   x = sgn(cos(u))*[cos(u)]^4 ;  y = sgn(sin(u))*[sin(u)]^4  ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =   80.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   40.0 ; Tn++ ;
  Ttypelist[Tn] = RZ ; Tvlist[Tn] =   45.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  130.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  650.0 ; Tn++ ;
// Graph the entire curve.
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(0, 1, 1);
	plot_shape(hyperbola_x, hyperbola_y, -1, 1.5, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// hyperbola :
//   x^2 - y^2 = 1 
//
//   u in (-inf,inf) gives the right branch :
//   x = cosh(u) ;   y = sinh(u) ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =   70.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   70.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  250.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  150.0 ; Tn++ ;
// Graph only the part with u in [-1, 1.5]  
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(1, 0, 1);
	plot_shape(parabola_x, parabola_y, -1, 2, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// parabola :
//   y = x^2
//
//   u in (inf,inf) gives the entire parabola : 
//   x = u ;  y  = u*u ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  150.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =   50.0 ; Tn++ ;
  Ttypelist[Tn] = RZ ; Tvlist[Tn] =   60.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  140.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  200.0 ; Tn++ ;
// Graph only the part with u in [-1, 2]
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(1, 1, 1);
	plot_shape(lemon_x, lemon_y, 0, 2 * M_PI, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// lemon :
//   x^2 - (1 - y^2)^3 = 0
//
//   u in [0,2*M_PI] gives the entire lemon :
//   x = [cos(u)]^3  ;   y = sin(u) ;
  Tn = 0 ; 
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  125.0 ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  125.0 ; Tn++ ;
  Ttypelist[Tn] = RZ ; Tvlist[Tn] =   60.0 ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  620.0 ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  210.0 ; Tn++ ;
// Graph the entire curve.
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;


	G_rgb(1, 1, 1);
	plot_shape(brachistochrone_x, brachistochrone_y, 0, 6 * M_PI, x[shape], y[shape], NUM_PTS);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// brachistochrone :
//   u in [0, tau] gives the entire curve
// x = u + cos(-pi/2 - u) ; y = 1 + sin(-pi/2 - u);
	Tn = 0;
	Ttypelist[Tn] = SX;	Tvlist[Tn] = 20;	Tn++;
	Ttypelist[Tn] = SY;	Tvlist[Tn] = 20;	Tn++;
	Ttypelist[Tn] = TX;	Tvlist[Tn] = 100;	Tn++;
	Ttypelist[Tn] = TY;	Tvlist[Tn] = 30;	Tn++;
// Graph the entire curve 3 times.
	M3d_make_movement_sequence_matrix(A, A_i, Tn, Ttypelist, Tvlist);
	M3d_mat_mult_points(x[shape], y[shape], z[shape], A, x[shape], y[shape], z[shape], NUM_PTS);
	graph(x[shape], y[shape], NUM_PTS);
	G_wait_key();
	shape++;

	while (1)
		if (G_wait_key() == 'q')
			return 0;
	return 0;
}
