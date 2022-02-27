#include "FPToolkit.c"
#include "xwd_tools_03.c"


int main()
{
  char nameA[100] ;
  int idA ;
  int widthA,heightA ;

  char nameB[100] ;
  int idB ;
  int widthB,heightB ;
  
  int d[2],w,h,x,y,e,q ;
  double rgb[3] ;

  printf("enter name of xwd file\n") ;
  scanf("%s",nameA) ;
  idA = init_xwd_map_from_file (nameA) ;// returns -1 on error, 1 if ok
  if (idA == -1) { printf("failure\n") ;  exit(0) ; }
  e = get_xwd_map_dimensions(idA, d) ;
  if (e == -1) { printf("failure\n") ;  exit(0) ; }
  widthA = d[0] ; heightA = d[1] ;


  printf("enter name of second xwd file\n") ;
  scanf("%s",nameB) ;
  idB = init_xwd_map_from_file (nameB) ;// returns -1 on error, 1 if ok
  if (idB == -1) { printf("failure\n") ;  exit(0) ; }
  e = get_xwd_map_dimensions(idB, d) ;
  if (e == -1) { printf("failure\n") ;  exit(0) ; }
  widthB = d[0] ; heightB = d[1] ;


  if ((widthA == widthB) && (heightA == heightB)) {
    w = widthA ;
    h = heightA ;
  } else {
    printf("The two images do not have the same dimensions.\n") ;
    exit(0) ;
  }


  G_init_graphics(w,h);
  
  for (x = 0 ; x < w ; x++) {
    for (y = 0 ; y < h ; y++) {

      if (x < w/2) {
        e = get_xwd_map_color(idA, x,y,rgb) ; // returns -1 on error, 1 if ok
        if (e == -1) { continue ; }
        G_rgb(0.5*rgb[0], 0.5*rgb[1], 0.5*rgb[2]) ; // blacker
      } else {
        e = get_xwd_map_color(idB, x,y,rgb) ; // returns -1 on error, 1 if ok
        if (e == -1) { continue ; }
        G_rgb(0.5*(1+rgb[0]), 0.5*(1+rgb[1]), 0.5*(1+rgb[2])) ; // whiter
      }

      G_point(x,y) ;
    }
  }

  q = G_wait_key() ;

}
