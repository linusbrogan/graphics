#include "FPToolkit.c"


int main ()
{
  int width,height,q ;
  char inputname[100] ;
  
  printf("enter window width and height ") ;
  scanf("%d %d",&width,&height) ;

  printf("enter name of xwd file to be read ") ;
  scanf("%s",inputname) ;

  G_init_graphics(width,height) ;


  G_get_image_from_file(inputname,100,50) ;

  q = G_wait_key() ;
}
