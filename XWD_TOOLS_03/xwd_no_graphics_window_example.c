#include "xwd_tools_03.c"

// No graphics window...no need for FPT
// directly write to files


int main()
{
  int w,h ;
  char name[100] ;
  int idA, idB ;
  double red,grn,blu ;
  int x,y,e ;

  w = 600 ; h = 300 ; 

  idA = create_new_xwd_map (w,h) ;// returns -1 on error, 1 if ok
  if (idA == -1) { printf("failure\n") ;  exit(0) ; }

  idB = create_new_xwd_map (h,w) ;// returns -1 on error, 1 if ok
  if (idB == -1) { printf("failure\n") ;  exit(0) ; }


  
  for (x = 0 ; x < w ; x++) {
    for (y = 0 ; y < h ; y++) {

      
      red = 1.0*x/w ;
      grn = 0.5 ;
      blu = 1.0*y/h ;

      e = set_xwd_map_color(idA, x,y, red,grn,blu) ;
              // returns -1 on error, 1 if ok

      e = set_xwd_map_color(idB, y,x, red,0.0,blu) ;
              // returns -1 on error, 1 if ok

    }
  }

  xwd_map_to_named_xwd_file(idA, "wider.xwd") ;
  xwd_map_to_named_xwd_file(idB, "taller.xwd") ;

}

