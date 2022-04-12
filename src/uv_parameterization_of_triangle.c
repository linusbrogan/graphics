#include <fpt.h>

int main()
{
  double u,v,A[2],B[2],C[2],AB[2],AC[2],P[2] ;
  G_init_graphics(800,800) ;
  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(0.5,0.5,0.5) ;
  G_fill_rectangle(0,0,60,30) ;
  
  
  G_rgb(1,1,0) ;  
  G_wait_click(A) ; G_fill_circle(A[0],A[1],2) ;  
  while (A[0] >= 60 || A[1] >= 20) {
    
  G_wait_click(B) ; G_fill_circle(B[0],B[1],2) ;  
  G_wait_click(C) ; G_fill_circle(C[0],C[1],2) ;

  G_rgb(0,1,0) ;
  G_triangle(A[0],A[1], B[0],B[1], C[0],C[1]) ;

  AB[0] = B[0] - A[0] ;  AB[1] = B[1] - A[1] ;
  AC[0] = C[0] - A[0] ;  AC[1] = C[1] - A[1] ;

  G_rgb(0,0,1) ;
  for (u = 0 ; u <= 1 ; u += 1.0/256) {
    for (v = 0 ; v <= 1 ; v += 1.0/256)  {

      P[0] = A[0] + u*AB[0] + v*AC[0] ;
      P[1] = A[1] + u*AB[1] + v*AC[1] ;
      G_point(P[0], P[1]) ;
      
    }
  }

  G_rgb(0,1,0) ;
  G_triangle(A[0],A[1], B[0],B[1], C[0],C[1]) ;

  
  G_rgb(1,1,0) ;  
  G_wait_click(A) ; G_fill_circle(A[0],A[1],2) ;    
  }

}
