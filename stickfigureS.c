
#include "FPToolkit.c"
#include "M3d_matrix_tools.c"

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

 
 // now make the movie the rotates and shrinks about the center :


 // .....

}

