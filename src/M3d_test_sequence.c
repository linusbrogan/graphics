#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "M3d_matrix_tools.c"


int main()
{
  double v[4][4], vi[4][4], p[4][4] ;
  int n ;
  int mtype[100] ;
  double mparam[100] ;

  n = 0 ;
  mtype[n] = SX ;  mparam[n] =  0.35 ; n++ ;
  mtype[n] = SZ ;  mparam[n] =  3.50 ; n++ ;
  mtype[n] = TY ;  mparam[n] =  2.44 ; n++ ;
  mtype[n] = NX ;  mparam[n] =     0 ; n++ ;  // parameter immaterial    
  mtype[n] = TX ;  mparam[n] = -7.15 ; n++ ;    
  mtype[n] = RY ;  mparam[n] =    42 ; n++ ;  // degrees
  mtype[n] = TZ ;  mparam[n] = -3.87 ; n++ ;
  mtype[n] = RX ;  mparam[n] =   -37 ; n++ ;  // degrees
  mtype[n] = NY ;  mparam[n] =     0 ; n++ ;  // parameter immaterial
  mtype[n] = RZ ;  mparam[n] =   -81 ; n++ ;  // degrees
  mtype[n] = SY ;  mparam[n] = -1.25 ; n++ ;
  mtype[n] = NZ ;  mparam[n] =     0 ; n++ ;  // parameter immaterial  

  M3d_make_movement_sequence_matrix(v,vi,  n,mtype,mparam) ;

  printf("v = \n") ;
  M3d_print_mat(v) ;
  printf("\n") ;

  printf("vi = \n") ;
  M3d_print_mat(vi) ;
  printf("\n") ;

  printf("v*vi = \n") ;
  M3d_mat_mult(p,  v,vi) ;
  M3d_print_mat(p) ;
  printf("\n") ;  
  
}
