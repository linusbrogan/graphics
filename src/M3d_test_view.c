#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <m3d.h>


int main()
{
  double v[4][4], vi[4][4] ;

  printf("\nTest1 :\n") ;
  double eyeA[3] = {10,20,15} ;
  double coiA[3] = {14,26,22} ;
  double upA[3]  = {11,32,17} ;

  M3d_view(v,vi,  eyeA,coiA,upA) ;
  M3d_print_mat(v) ;
  printf("\n") ;
  M3d_print_mat(vi) ;	     

  printf("\nTest2 :\n") ;  
  double eyeB[3] = {10,-20,15} ;
  double coiB[3] = {14,26,22} ;
  double upB[3]  = {11,32,17} ;

  M3d_view(v,vi,  eyeB,coiB,upB) ;
  M3d_print_mat(v) ;
  printf("\n") ;
  M3d_print_mat(vi) ;	     
  
}
