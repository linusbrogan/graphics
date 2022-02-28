#include <fpt.h>
#include <m3d.h>

// stickfigure initially designed as centered for a 400x400 window :
double x[13] = {175,225,225,300,225,225,250,200,150,175,175,100,175} ;
double y[13] = {300,300,250,225,225,200,100,175,100,200,225,225,250} ;
double z[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0} ;
       // z[] values unimportant but should NOT be left uninitialized
       // as nan values WILL propagate through
int n = 13 ;



int main(int argc, char **argv) 
{

 if (argc != 3) {
    printf("usage : pgm_name   window_size  microseconds(30000)\n") ;
    exit(0) ;
 }
  
 double winsize = atoi(argv[1]) ;
 int u = atoi(argv[2]) ; 

 G_init_graphics(winsize,winsize) ;

 // the original design was for a 400x400
 // window and the object is centered on 200,200
 // so we recenter it and make it larger
 // (you get to do this ... use the
 // M3d_make_movement_sequence_matrix  function :)

 // .....
// recenter at O, scale, recenter at mid
	int mtype[100];
	double mparam[100];
	int n = 0;
	mtype[n] = TX;	mparam[n] = -200;	n++;
	mtype[n] = TY;	mparam[n] = -200;	n++;
	double scale = winsize / 400;
	mtype[n] = SX;	mparam[n] = scale;	n++;
	mtype[n] = SY;	mparam[n] = scale;	n++;
	double center = winsize / 2;
	mtype[n] = TX;	mparam[n] = center;	n++;
	mtype[n] = TY;	mparam[n] = center;	n++;

	double recenter[4][4];
	double recenter_i[4][4];
	M3d_make_movement_sequence_matrix(recenter, recenter_i, n, mtype, mparam);
	M3d_mat_mult_points(x, y, z, recenter, x, y, z, 13);

	n = 0;
	mtype[n] = TX;	mparam[n] = -center;	n++;
	mtype[n] = TY;	mparam[n] = -center;	n++;
	mtype[n] = SX;	mparam[n] = 0.9;	n++;
	mtype[n] = SY;	mparam[n] = 0.9;	n++;
	mtype[n] = RZ;	mparam[n] = 10;	n++;
	mtype[n] = TX;	mparam[n] = center;	n++;
	mtype[n] = TY;	mparam[n] = center;	n++;

	double fall[4][4];
	double fall_i[4][4];
	M3d_make_movement_sequence_matrix(fall, fall_i, n, mtype, mparam);


 // now make the movie the rotates and shrinks about the center :
//spin + direction, shrinking until very small, then red circle
	G_rgb(0, 0, 0);
	G_clear();

	G_rgb(0.25, 0.5, 1);
	G_fill_polygon(x, y, 13);
	G_wait_key();

	int i = 0;
	while (i < 50) {
		G_rgb(0, 0, 0);
		G_clear();

		G_rgb(0.25, 0.5, 1);
		G_fill_polygon(x, y, 13);

		G_display_image();
		usleep(u);
		char keypress = G_no_wait_key();
		if (keypress == 'q') exit(0);

		M3d_mat_mult_points(x, y, z, fall, x, y, z, 13);
		i++;
	}
	G_rgb(0, 0, 0);
	G_clear();
	G_rgb(1, 0, 0);
	G_fill_circle(center, center, center / 6);
	while (1)
		if (G_wait_key() == 'q')
			break;
 // .....

}

