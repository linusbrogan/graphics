


  double halfangle_degrees = 40 ;

  
  double eye[3], coi[3], up[3] ;
  eye[0] = 0 ; eye[1] = 400 ; eye[2] = -1000 ;
  coi[0] = 0 ; coi[1] =  0 ; coi[2] =   0 ;
  up[0] = eye[0] ; up[1] = eye[1]+1 ; up[2] = eye[2] ;
  M3d_view(vm,vi,  eye,coi,up) ;

  double light_in_world_space[3] = { 100, 300, 50 } ;
  M3d_mat_mult_pt(light_in_eye_space,   vm, light_in_world_space) ;
  AMBIENT = 0.20 ;
  MAX_DIFFUSE = 0.50 ;
  SPECPOW = 50 ;
  
  int nn,ii ;
    
    num_objects = 0 ;

    //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////
    obtype[num_objects] = 1 ; // sphere
    color[num_objects][0] = 0.8 ;
    color[num_objects][1] = 0.8 ; 
    color[num_objects][2] = 0.8 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  800  ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =   10   ; Tn++ ;
    Ttypelist[Tn] = SZ ; Tvlist[Tn] =  800   ; Tn++ ;    
    Ttypelist[Tn] = TY ; Tvlist[Tn] = -150   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this


    //////////////////////////////////////////////////////////////
    nn = 3 ;
    for (ii = 0 ; ii < nn ; ii++) {
    obtype[num_objects] = 1 ; // sphere
    color[num_objects][0] = 1.0 ;
    color[num_objects][1] = 0.8 ; 
    color[num_objects][2] = 0.8 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =  500  ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =  100   ; Tn++ ;
    Ttypelist[Tn] = SZ ; Tvlist[Tn] =   80   ; Tn++ ;
    Ttypelist[Tn] = RY ; Tvlist[Tn] =  ii*360/nn   ; Tn++ ;        
    Ttypelist[Tn] = TY ; Tvlist[Tn] =  650   ; Tn++ ;
	
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this
    }
    

    //////////////////////////////////////////////////////////////

    nn = 6 ;
    for (ii = 0 ; ii < nn ; ii++) {
      
    obtype[num_objects] = 2 ; // hyperboloid
    color[num_objects][0] = 0.8 ;
    color[num_objects][1] = 0.4 ;
    color[num_objects][2] = 0.2 ;
	
    Tn = 0 ;
    Ttypelist[Tn] = SX ; Tvlist[Tn] =   40   ; Tn++ ;
    Ttypelist[Tn] = SY ; Tvlist[Tn] =  150   ; Tn++ ;
    Ttypelist[Tn] = SZ ; Tvlist[Tn] =   40   ; Tn++ ;    
    Ttypelist[Tn] = TX ; Tvlist[Tn] =  600  ; Tn++ ;
    Ttypelist[Tn] = RY ; Tvlist[Tn] =  ii*360/nn  ; Tn++ ;    
    
    M3d_make_movement_sequence_matrix(m, mi, Tn, Ttypelist, Tvlist);
    M3d_mat_mult(obmat[num_objects], vm, m) ;
    M3d_mat_mult(obinv[num_objects], mi, vi) ;

    num_objects++ ; // don't forget to do this

    }

