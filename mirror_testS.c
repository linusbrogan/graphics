#include "../FPToolkit.c"
#include "../M3d_matrix_tools.c"

//Support Eye
double eye[3];
double coi[3];
double up[3];
int    scrnsize = 800;
double hither = 1;
double yon = 1e50;

//Support Objects
int    num_objects ;
double obmat[100][4][4] ;
double obinv[100][4][4] ;
int    obtype[100]; //0 = sphere, 1 = square(z=0, x and y in [-1,1]), 2 = hyperbaloid
double color[100][3] ;
double objreflectivity[100]; //[0,1] percent reflectivity, -1 for no light/reflection model

//Support Light model
double light_in_world_space[3] = {0,20,30};
double light_in_eye_space[3];
double AMBIENT      = 0.2 ;
double MAX_DIFFUSE  = 0.5 ;
double SPECPOW      = 50 ;


int create_object_matricies(double vm[4][4], double vi[4][4]){
  //vm = view matrix
  //vi = view matrix inverse

  double Tvlist[100];
  int Tn, Ttypelist[100];
  double m[4][4], mi[4][4];

  
  num_objects = 0 ;
    
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  obtype[num_objects] = 2;
  color[num_objects][0] = 0.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  55   ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////

  obtype[num_objects] = 2;
  color[num_objects][0] = 0.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  25   ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////
  
  //////////////////////////////////////////////////////////////

  obtype[num_objects] = 2;
  color[num_objects][0] = 0.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = TX ; Tvlist[Tn] =  -25   ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////

  obtype[num_objects] = 2;
  color[num_objects][0] = 0.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  5    ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////

  //Chandelier
  obtype[num_objects] = 0;
  color[num_objects][0] = 0.7 ;
  color[num_objects][1] = 0.7 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  1    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  30    ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  25    ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////

  obtype[num_objects] = 0;
  color[num_objects][0] = 0.7 ;
  color[num_objects][1] = 0.7 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  1    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  2    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  10    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  30    ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  25    ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////
  
  //MIRRORS
    
  obtype[num_objects] = 1;
  color[num_objects][0] = 1.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 0.0 ;
  objreflectivity[num_objects] = 0.8;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = RX ; Tvlist[Tn] =  89   ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  30   ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  -10.2   ; Tn++ ;
    
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////
    
  obtype[num_objects] = 1;
  color[num_objects][0] = 0.0 ;
  color[num_objects][1] = 0.8 ; 
  color[num_objects][2] = 1.0 ;
  objreflectivity[num_objects] = 0.8;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  20   ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  20   ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  20   ; Tn++ ;
  Ttypelist[Tn] = RX ; Tvlist[Tn] =  150   ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  55   ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  20   ; Tn++ ;
    
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////
  
  obtype[num_objects] = 0;
  color[num_objects][0] = 0.2 ;
  color[num_objects][1] = 0.2 ; 
  color[num_objects][2] = 0.2 ;
  objreflectivity[num_objects] = -1;
	
  Tn = 0 ;
  Ttypelist[Tn] = SX ; Tvlist[Tn] =  100    ; Tn++ ;
  Ttypelist[Tn] = SY ; Tvlist[Tn] =  100    ; Tn++ ;
  Ttypelist[Tn] = SZ ; Tvlist[Tn] =  100    ; Tn++ ;
  Ttypelist[Tn] = TZ ; Tvlist[Tn] =  50    ; Tn++ ;
  Ttypelist[Tn] = TY ; Tvlist[Tn] =  0    ; Tn++ ;
	
  M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
  M3d_mat_mult(obmat[num_objects], vm, m) ;
  M3d_mat_mult(obinv[num_objects], mi, vi) ;

  num_objects++ ; // don't forget to do this
  //////////////////////////////////////////////////////////////
  return 1;
}

int test01()
{
  eye[0] = 0;
  eye[1] = 0;
  eye[2] = -10;
  coi[0] = 0;
  coi[1] = 0;
  coi[2] = 50;
  up[0] = 0;
  up[1] = 1;
  up[2] = 0;
  //////////////////////////////////////////////////////////////////////

  
  G_rgb(0,0,0) ;
  G_clear() ;
    
  double t = 0;
  int c;
  while(1){
    t += 0.1;
    //move the eye!
    eye[0] = 25*cos(M_PI + t);
    eye[1] = 25*sin(M_PI + t) + 25;
    
    up[0] = eye[0];
    up[1] = eye[1] + 1;
    up[2] = eye[2];

    //draw it!   
  }
}

int main()
{
  G_init_graphics(scrnsize,scrnsize);
  test01() ;
}

