
// read the data from multiple xyz files and display all of the objects

#include "FPToolkit.c"
#include "M3d_matrix_tools.c"


int window_width, window_height, window_square_size ;
double Half_window_size ;
double Half_angle_degrees ;
double Tan_half_angle ;



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// M controls the maximum number of objects :
#define M 30
int numobjects ;
int numpoints[M] ;
int numpolys[M] ;
double x[M][10000] ;
double y[M][10000] ;
double z[M][10000] ;
int psize[M][5000] ;
int con[M][5000][20] ; // no polygon more than 20 verts
double inherent_rgb[M][3] ;



int read_xyz_file_into_data_structure (FILE *f, int onum)
{
  int i,polynum,j ;

  // point info
  fscanf(f,"%d",&numpoints[onum]) ;
  for (i = 0 ; i < numpoints[onum] ; i++) {
    fscanf(f,"%lf %lf %lf",&x[onum][i],&y[onum][i],&z[onum][i]) ;
  }


  // connectivity info
  fscanf(f,"%d",&numpolys[onum]) ;
  for (polynum = 0 ; polynum < numpolys[onum] ; polynum++) {

    fscanf(f,"%d",&psize[onum][polynum]) ;

    for (j = 0 ; j < psize[onum][polynum] ; j++) {
      fscanf(f,"%d",&con[onum][polynum][j]) ;
    }
  }

}




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// To support the light model :
double light_in_eye_space[3] ;
double AMBIENT      = 0.2 ;
double MAX_DIFFUSE  = 0.5 ;
double SPECPOW      = 50 ;



int Light_Model (double irgb[3],
                 double s[3],
                 double p[3],
                 double n[3],
                 double argb[3])
// s,p,n in eyespace

// irgb == inherent color of object (input to this function)
// s = location of start of ray (probably the eye)
// p = point on object (input to this function)
// n = normal to the object at p (input to this function)
// argb == actual color of object (output of this function)
// globals : AMBIENT, MAX_DIFFUSE, SPECPOW, light_in_eye_space[3]

// return 1 if successful, 0 if error
{

  double len ;
  double N[3] ; 
  len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]) ;
  if (len == 0) return 0 ;
  N[0] = n[0]/len ;  N[1] = n[1]/len ;  N[2] = n[2]/len ;

  double E[3] ;
  E[0] = s[0] - p[0] ; 
  E[1] = s[1] - p[1] ; 
  E[2] = s[2] - p[2] ; 
  len = sqrt(E[0]*E[0] + E[1]*E[1] + E[2]*E[2]) ;
  if (len == 0) return 0 ;
  E[0] /= len ;  E[1] /= len ;  E[2] /= len ;
  double NdotE = N[0]*E[0] + N[1]*E[1] + N[2]*E[2] ;

  double L[3] ;
  L[0] = light_in_eye_space[0] - p[0] ; 
  L[1] = light_in_eye_space[1] - p[1] ; 
  L[2] = light_in_eye_space[2] - p[2] ; 
  len = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]) ;
  if (len == 0) return 0 ;
  L[0] /= len ;  L[1] /= len ;  L[2] /= len ;
  double NdotL = N[0]*L[0] + N[1]*L[1] + N[2]*L[2] ;





  double max_ambient_and_diffuse = AMBIENT + MAX_DIFFUSE ;
     // this needs to occur BEFORE you possibly jump to LLL below




  double intensity ;
  if (NdotL*NdotE < 0) {
    // eye and light are on opposite sides of polygon
    intensity = AMBIENT ; 
    goto LLL ;
  } else if ((NdotL < 0) && (NdotE < 0)) {
    // eye and light on same side but normal pointing "wrong" way
    N[0] *= (-1.0) ;    N[1] *= (-1.0) ;    N[2] *= (-1.0) ; 
    NdotL *= (-1.0) ;
    NdotE *= (-1.0) ;   // don't use NdotE below, probably should eliminate this
  }


  // ignore Blinn's variant
  double R[3] ; // Reflection vector of incoming light
  R[0] = 2*NdotL*N[0] - L[0] ;
  R[1] = 2*NdotL*N[1] - L[1] ;
  R[2] = 2*NdotL*N[2] - L[2] ;

  double EdotR = E[0]*R[0] + E[1]*R[1] + E[2]*R[2] ;

  double diffuse ;
  if (NdotL <= 0.0) { diffuse = 0.0 ; }
  else { diffuse = MAX_DIFFUSE*NdotL ; }

  double specular ;
  if (EdotR <= 0.0) { specular = 0.0 ; }
  else { specular = (1.0 - max_ambient_and_diffuse)*pow(EdotR,SPECPOW) ;}

  // printf("%lf %lf\n",diffuse,specular) ;
  intensity = AMBIENT + diffuse + specular ;



 LLL : ;

  double f,g ;
  if (intensity <= max_ambient_and_diffuse) {
    f = intensity / max_ambient_and_diffuse ;
    argb[0] = f * irgb[0] ;
    argb[1] = f * irgb[1] ;
    argb[2] = f * irgb[2] ;
  } else {
    f = (intensity - max_ambient_and_diffuse) / 
                           (1.0 - max_ambient_and_diffuse) ;
    g = 1.0 - f ;
    argb[0] = g * irgb[0] + f ;
    argb[1] = g * irgb[1] + f ;
    argb[2] = g * irgb[2] + f ;
  }

  return 1 ;
}






void light_model (double irgb[3],
                  double *xx, double *yy, double *zz, 
                  double argb[3])
// irgb == inherent color of object (input to this function)
// xx[],yy[],zz[] are points in the polygon
// argb == actual color of object (output of this function)
{
  double Eye[3] ;
  Eye[0] = 0 ; Eye[1] = 0 ; Eye[2] = 0 ; 

  double P[3]  ;
  P[0] = xx[0] ;  P[1] = yy[0] ;  P[2] = zz[0] ;

  double a[3] ;
  a[0] = xx[1] - xx[0] ;  a[1] = yy[1] - yy[0] ;  a[2] = zz[1] - zz[0] ;

  double b[3] ;
  b[0] = xx[2] - xx[0] ;  b[1] = yy[2] - yy[0] ;  b[2] = zz[2] - zz[0] ;
 
  double N[3] ;
  M3d_x_product (N, a,b) ;

  Light_Model (irgb, Eye, P, N, argb) ;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






// To support Painter's algorithm :
typedef
struct {
  int onum ;
  int pnum ;
  double dist ;
}
PINFO ;

PINFO polyinfo[20000] ;

int PINFO_compare (const void *p, const void *q)
{
  PINFO *a, *b ;

  a = (PINFO*)p ;
  b = (PINFO*)q ;

  // remember we want furthest distance FIRST
  if      (((*a).dist) < ((*b).dist)) return  1 ;
  else if (((*a).dist) > ((*b).dist)) return -1 ;
  else                                return  0 ;
}



void draw_all_objects()
{
  double a,b,c ;
  int onum, pnum,j,size,d ;
  double xp[100],yp[100] ;

  int TotalPolys ;
  double sumx,sumy,sumz ;

  a = Half_window_size / Tan_half_angle ;
  b = 0.5*window_width ;
  c = 0.5*window_height ;

  TotalPolys = 0 ;

  for (onum = 0 ; onum < numobjects ; onum++) {

      for (pnum = 0 ; pnum < numpolys[onum] ; pnum++) {

        size = psize[onum][pnum] ;

	sumx = sumy = sumz = 0.0 ;
        for (j = 0 ; j < size ; j++) {
          d = con[onum][pnum][j] ;

          sumx += x[onum][d] ;
          sumy += y[onum][d] ;
          sumz += z[onum][d] ;

        }
	sumx /= size ;
	sumy /= size ;
	sumz /= size ;
	polyinfo[TotalPolys].onum = onum ;
	polyinfo[TotalPolys].pnum = pnum ;
	polyinfo[TotalPolys].dist = sqrt(sumx*sumx + sumy*sumy + sumz*sumz) ;
	TotalPolys++ ;

      } // end for pnum

  } // end for onum


  // printf("TotalPolys = %d\n",TotalPolys) ;
  ///////////////////////////////////////
  qsort(polyinfo, TotalPolys, sizeof(PINFO), PINFO_compare) ;
  ///////////////////////////////////////

  double xx[100],yy[100],zz[100] ;
  double rgb[3] ;
  int k ;
  for (k = 0 ; k < TotalPolys ; k++) {

    onum = polyinfo[k].onum ;
    pnum = polyinfo[k].pnum ;

        size = psize[onum][pnum] ;

        for (j = 0 ; j < size ; j++) {
          d = con[onum][pnum][j] ;

          xx[j] = x[onum][d] ;
          yy[j] = y[onum][d] ;
          zz[j] = z[onum][d] ;

          xp[j] = a*(xx[j]/zz[j]) + b ;
          yp[j] = a*(yy[j]/zz[j]) + c ;
        }


        light_model (inherent_rgb[onum], xx,yy,zz, rgb) ;

	G_rgb(rgb[0], rgb[1], rgb[2]) ;
	// printf("%lf %lf %lf\n",
	// inherent_rgb[onum][0],inherent_rgb[onum][2],inherent_rgb[onum][2]) ;
        // printf("%lf %lf %lf\n",rgb[0],rgb[1],rgb[2]) ;

        G_fill_polygon(xp,yp,size) ;
	G_rgb(0,0,0) ;
	// G_polygon(xp,yp,size) ;

  }

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int main ()
{
  FILE *f ;
  double V[4][4], Vi[4][4] ;
  int onum, q ;

  int nl ;
  int tlist[100] ;
  double plist[100] ;



  window_width = 600 ; window_height = 600 ;
  // size of largest square INside window
  if (window_width < window_height) { window_square_size = window_width ; }
                               else { window_square_size = window_height ; }
  Half_window_size = 0.5*window_square_size ;
  Half_angle_degrees = 30 ;
  Tan_half_angle = tan(Half_angle_degrees*M_PI/180) ;
  G_init_graphics (window_width, window_height) ;



  light_in_eye_space[0] =  100 ;
  light_in_eye_space[1] =  100 ;
  light_in_eye_space[2] = -100 ;
  
  AMBIENT = 0.2 ;
  MAX_DIFFUSE = 0.5 ;
  SPECPOW = 30 ;




  //---------------------------------------------------------------------
  // Design the model by moving objects from their own OBJECT SPACE 
  // to a common WORLD SPACE :
  //---------------------------------------------------------------------

  onum = 0 ;  // current object number

  // The sphere FILE has a radius of 1.0, centered at the origin.
  // The cylinder FILE has radius of 0.1, centered at origin, 
  // lying on the x-axis from x = -1 to x = 1.


  // Build an origin point out of the sphere file.
  f = fopen("sphere.xyz","r") ;
  if (f == NULL) { printf("Can't open file %s\n", "sphere.xyz") ;  exit(0) ;  }
  inherent_rgb[onum][0] = 1.0 ;
  inherent_rgb[onum][1] = 0.8 ;
  inherent_rgb[onum][2] = 0.0 ;
  read_xyz_file_into_data_structure (f, onum) ;
  fclose(f) ;

  nl = 0 ;
  tlist[nl] = SX ; plist[nl] = 0.25 ; nl++ ;
  tlist[nl] = SY ; plist[nl] = 0.25 ; nl++ ;
  tlist[nl] = SZ ; plist[nl] = 0.25 ; nl++ ;  
  M3d_make_movement_sequence_matrix (V,Vi,  nl,tlist,plist) ;

  M3d_mat_mult_points (x[onum],y[onum],z[onum],
		       V,  x[onum],y[onum],z[onum],numpoints[onum]) ;

  onum++ ;


  // Build a +x axis with the cylinder file.
  f = fopen("cylinder.xyz","r") ;
  if (f == NULL) { printf("Can't open file %s\n", "cylinder.xyz") ;  exit(0) ;  }
  inherent_rgb[onum][0] = 1.0 ;
  inherent_rgb[onum][1] = 0.2 ;
  inherent_rgb[onum][2] = 0.2 ;
  read_xyz_file_into_data_structure (f, onum) ;
  fclose(f) ;

  nl = 0 ;
  tlist[nl] = TX ; plist[nl] = 1.00 ; nl++ ;
  tlist[nl] = SX ; plist[nl] = 2.00 ; nl++ ;
  M3d_make_movement_sequence_matrix (V,Vi,  nl,tlist,plist) ;
  
  M3d_mat_mult_points (x[onum],y[onum],z[onum],
		       V,  x[onum],y[onum],z[onum],numpoints[onum]) ;

  onum++ ;


  // Build a +y axis with the cylinder file.
  f = fopen("cylinder.xyz","r") ;
  if (f == NULL) { printf("Can't open file %s\n", "cylinder.xyz") ;  exit(0) ;  }
  inherent_rgb[onum][0] = 1.0 ;
  inherent_rgb[onum][1] = 1.0 ;
  inherent_rgb[onum][2] = 1.0 ;
  read_xyz_file_into_data_structure (f, onum) ;
  fclose(f) ;
  
  nl = 0 ;
  tlist[nl] = TX ; plist[nl] = 1.00 ; nl++ ;
  tlist[nl] = SX ; plist[nl] = 2.00 ; nl++ ;
  tlist[nl] = RZ ; plist[nl] =  90  ; nl++ ;
  M3d_make_movement_sequence_matrix (V,Vi,  nl,tlist,plist) ;
  
  M3d_mat_mult_points (x[onum],y[onum],z[onum],
		       V,  x[onum],y[onum],z[onum],numpoints[onum]) ;

  onum++ ;


  // Build a +z axis with a cylinder.
  f = fopen("cylinder.xyz","r") ;
  if (f == NULL) { printf("Can't open file %s\n", "cylinder.xyz") ;  exit(0) ;  }
  inherent_rgb[onum][0] = 0.3 ;
  inherent_rgb[onum][1] = 0.2 ;
  inherent_rgb[onum][2] = 1.0 ;
  read_xyz_file_into_data_structure (f, onum) ;
  fclose(f) ;

  nl = 0 ;
  tlist[nl] = TX ; plist[nl] = 1.00 ; nl++ ;
  tlist[nl] = SX ; plist[nl] = 2.00 ; nl++ ;
  tlist[nl] = RY ; plist[nl] = -90  ; nl++ ;
  M3d_make_movement_sequence_matrix (V,Vi,  nl,tlist,plist) ;  

  M3d_mat_mult_points (x[onum],y[onum],z[onum],
		       V,  x[onum],y[onum],z[onum],numpoints[onum]) ;

  onum++ ;



  //---------------------------------------------------------------------
  numobjects = onum ;
  //---------------------------------------------------------------------

  double eye[3], coi[3], up[3] ;
  eye[0] = 10 ; eye[1] =  5  ; eye[2] =  4 ;
  coi[0] =  2 ; coi[1] = -1  ; coi[2] =  3 ;
  up[0]  = 11 ; up[1]  = 13  ; up[2]  =  6 ;
  M3d_view (V, Vi,  eye,coi,up) ;


  // move ALL objects from WORLD SPACE into EYE SPACE :
  for (onum = 0 ; onum < numobjects ; onum++) {
    M3d_mat_mult_points (x[onum],y[onum],z[onum],  V, 
                         x[onum],y[onum],z[onum],numpoints[onum]) ;
  }

  G_rgb(0,0,0) ; 
  G_clear() ;
  draw_all_objects() ;
  G_save_image_to_file("view_test04Mb0000.xwd") ;

  q = G_wait_key() ;

}

