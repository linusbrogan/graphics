#include <stdio.h>
#include <math.h>


/*

 ( x')          (x)
 ( y')  =   M * (y)  
 ( z')          (z)
 ( 1 )          (1)

instead of (x',y',z',1) = (x,y,z,1) * M  

*/




int M3d_print_mat (double a[4][4])
{
  int r,c ;
  for (r = 0 ; r < 4 ; r++ ) {
      for (c = 0 ; c < 4 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1 ;
} 





int M3d_copy_mat (double a[4][4], double b[4][4])
// a = b
{
  int r,c ;
  for (r = 0 ; r < 4 ; r++ ) {
      for (c = 0 ; c < 4 ; c++ ) {
           a[r][c] = b[r][c] ;
      }
  }

  return 1 ;
} 





int M3d_make_identity (double a[4][4])
// a = I
{
  int r,c ;
  for (r = 0 ; r < 4 ; r++ ) {
      for (c = 0 ; c < 4 ; c++ ) {
           if (r == c) a[r][c] = 1.0 ;
               else    a[r][c] = 0.0 ;
      }
  }

  return 1 ;
} 





int M3d_make_translation (double a[4][4], double dx, double dy, double dz)
{
  M3d_make_identity(a) ;
  a[0][3] =  dx ;  a[1][3] = dy ;  a[2][3] = dz ;
  return 1 ;
}





int M3d_make_scaling (double a[4][4], double sx, double sy, double sz)
{
  M3d_make_identity(a) ;
  a[0][0] =  sx ;  a[1][1] = sy ;  a[2][2] = sz ;
  return 1 ;
}












int M3d_make_x_rotation_cs (double a[4][4], double cs, double sn)
// this one assumes cosine and sine are already known
{
  M3d_make_identity(a) ;

  a[1][1] =   cs ;  a[1][2] = -sn ;
  a[2][1] =   sn ;  a[2][2] =  cs ;

  return 1 ;
}



int M3d_make_y_rotation_cs (double a[4][4], double cs, double sn)
// this one assumes cosine and sine are already known
{
  M3d_make_identity(a) ;

  a[0][0] =   cs ;  a[0][2] =  sn ;
  a[2][0] =  -sn ;  a[2][2] =  cs ;

  return 1 ;
}


int M3d_make_z_rotation_cs (double a[4][4], double cs, double sn)
// this one assumes cosine and sine are already known
{
  M3d_make_identity(a) ;

  a[0][0] =   cs ;  a[0][1] = -sn ;
  a[1][0] =   sn ;  a[1][1] =  cs ;

  return 1 ;
}


int M3d_make_shear(
	double a[4][4],
	double xy,
	double xz,
	double yx,
	double yz,
	double zx,
	double zy
) {
	M3d_make_identity(a);
	a[0][1] = xy;
	a[0][2] = xz;
	a[1][0] = yx;
	a[1][2] = yz;
	a[2][0] = zx;
	a[2][1] = zy;
	return 1;
}



int M3d_mat_mult (double res[4][4], double a[4][4], double b[4][4])
// res = a * b
// this is SAFE, i.e. the user can make a call such as 
// M3d_mat_mult(p,  p,q) or M3d_mat_mult(p,  q,p) or  M3d_mat_mult(p, p,p)
{
  double sum ;
  int k ;
  int r,c ;
  double tmp[4][4] ;

  for (r = 0 ; r < 4 ; r++ ) {
      for (c = 0 ; c < 4 ; c++ ) {
           sum = 0.0 ;
           for (k = 0 ; k < 4 ; k++) {
                 sum = sum + a[r][k]*b[k][c] ;
           }
           tmp[r][c] = sum ;
      }
  }


  M3d_copy_mat (res,tmp) ;

  return 1 ;
}





int M3d_mat_mult_pt (double P[3],   double m[4][4], double Q[3])
// P = m*Q
// SAFE, user may make a call like M3d_mat_mult_pt (W, m,W) ;
{
  double u,v,t ;

  u = m[0][0]*Q[0] + m[0][1]*Q[1] + m[0][2]*Q[2] + m[0][3] ;
  v = m[1][0]*Q[0] + m[1][1]*Q[1] + m[1][2]*Q[2] + m[1][3] ;
  t = m[2][0]*Q[0] + m[2][1]*Q[1] + m[2][2]*Q[2] + m[2][3] ;  

  P[0] = u ;
  P[1] = v ;
  P[2] = t ;
  
  return 1 ;
}





int M3d_mat_mult_points (double X[], double Y[], double Z[],
                         double m[4][4],
                         double x[], double y[], double z[], int numpoints)
// |X0 X1 X2 ...|       |x0 x1 x2 ...|
// |Y0 Y1 Y2 ...| = m * |y0 y1 y2 ...|
// |Z0 Z1 Z2 ...|       |z0 z1 z2 ...|  
// | 1  1  1 ...|       | 1  1  1 ...|

// SAFE, user may make a call like M3d_mat_mult_points (x,y,z,  m, x,y,z,  n) ;
{
  double u,v,t ;
  int i ;

  for (i = 0 ; i < numpoints ; i++) {
    u = m[0][0]*x[i] + m[0][1]*y[i] + m[0][2]*z[i] + m[0][3] ;
    v = m[1][0]*x[i] + m[1][1]*y[i] + m[1][2]*z[i] + m[1][3] ;
    t = m[2][0]*x[i] + m[2][1]*y[i] + m[2][2]*z[i] + m[2][3] ;    

    X[i] = u ;
    Y[i] = v ;
    Z[i] = t ;
  }

  return 1 ;
}






int M3d_x_product (double res[3], double a[3], double b[3])
// res = a x b  , cross product of two vectors
// SAFE: it is ok to make a call such as
// D3d_x_product (a,  a,b) or
// D3d_x_product (b,  a,b) or
// D3d_x_product (a,  a,a) 
{
    double r[3] ;
    int v ;
    
    r[0] = a[1]*b[2] - b[1]*a[2] ;
    r[1] = b[0]*a[2] - a[0]*b[2] ;
    r[2] = a[0]*b[1] - b[0]*a[1] ;

    res[0] = r[0] ;
    res[1] = r[1] ;
    res[2] = r[2] ;

    if ((res[0] == 0) && (res[1] == 0) && (res[2] == 0)) {
	v = 0 ;
    } else {
	v = 1 ;
    }

    return v ;
}






//===========================================================================
// For Advanced Graphics :
//===========================================================================




enum transformation {
	SX,
	SY,
	SZ,
	RX,
	RY,
	RZ,
	TX,
	TY,
	TZ,
	NX,
	NY,
	NZ,
	HXY,
	HXZ,
	HYX,
	HYZ,
	HZX,
	HZY
};



//////////////

#define DEGREES (((M_PI) / (180)))

int fill_action_matrix(double result[4][4], int action_type, double parameter) {
	switch (action_type) {
		case SX:
		return M3d_make_scaling(result, parameter, 1, 1);
		case SY:
		return M3d_make_scaling(result, 1, parameter, 1);
		case SZ:
		return M3d_make_scaling(result, 1, 1, parameter);

		case RX:
		return M3d_make_x_rotation_cs(result, cos(parameter * DEGREES), sin(parameter * DEGREES));
		case RY:
		return M3d_make_y_rotation_cs(result, cos(parameter * DEGREES), sin(parameter * DEGREES));
		case RZ:
		return M3d_make_z_rotation_cs(result, cos(parameter * DEGREES), sin(parameter * DEGREES));

		case TX:
		return M3d_make_translation(result, parameter, 0, 0);
		case TY:
		return M3d_make_translation(result, 0, parameter, 0);
		case TZ:
		return M3d_make_translation(result, 0, 0, parameter);

		case NX:
		return M3d_make_scaling(result, -1, 1, 1);
		case NY:
		return M3d_make_scaling(result, 1, -1, 1);
		case NZ:
		return M3d_make_scaling(result, 1, 1, -1);

		case HXY:
		return M3d_make_shear(result, parameter, 0, 0, 0, 0, 0);
		case HXZ:
		return M3d_make_shear(result, 0, parameter, 0, 0, 0, 0);
		case HYX:
		return M3d_make_shear(result, 0, 0, parameter, 0, 0, 0);
		case HYZ:
		return M3d_make_shear(result, 0, 0, 0, parameter, 0, 0);
		case HZX:
		return M3d_make_shear(result, 0, 0, 0, 0, parameter, 0);
		case HZY:
		return M3d_make_shear(result, 0, 0, 0, 0, 0, parameter);

		default:
		return 0;
	}
}

int fill_action_matrix_inverse(double result[4][4], int action_type, double parameter) {
	switch (action_type) {
		case SX:
		case SY:
		case SZ:
		parameter = 1 / parameter;
		break;

		case RX:
		case RY:
		case RZ:
		case TX:
		case TY:
		case TZ:
		case HXY:
		case HXZ:
		case HYX:
		case HYZ:
		case HZX:
		case HZY:
		parameter = -parameter;
	}
	return fill_action_matrix(result, action_type, parameter);
}

int M3d_make_movement_sequence_matrix(double v[4][4], double vi[4][4], int n, int mtype[], double mparam[]) {
	double A[4][4];
	M3d_make_identity(A);
	double B[4][4];
	M3d_make_identity(B);

	for (int i = 0; i < n; i++) {
		double temp[4][4];
		fill_action_matrix(temp, mtype[i], mparam[i]);
		M3d_mat_mult(A, temp, A);
		fill_action_matrix_inverse(temp, mtype[i], mparam[i]);
		M3d_mat_mult(B, B, temp);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			v[i][j] = A[i][j];
			vi[i][j] = B[i][j];
		}
	}

	return 1;
}


/////////////////////

int M3d_view(double view[4][4], double view_inverse[4][4], double eye[3], double coi[3], double up[3]) {
	double A[4][4];
	double B[4][4];
	double C[4][4];
	double D[4][4];
	double Ai[4][4];
	double Bi[4][4];
	double Ci[4][4];
	double Di[4][4];
	double Up[3];
	double Uplen;

	double a = coi[0] - eye[0];
	double b = coi[1] - eye[1];
	double c = coi[2] - eye[2];
	double p = sqrt(a * a + c * c);
	double r = sqrt(b * b + p * p);

	M3d_make_translation(A, -eye[0], -eye[1], -eye[2]);
	M3d_make_y_rotation_cs(B, c/p, -a/p);
	M3d_make_x_rotation_cs(C, p/r, b/r);

	M3d_make_identity(view);
	M3d_mat_mult(view, A, view);
	M3d_mat_mult(view, B, view);
	M3d_mat_mult(view, C, view);

	M3d_mat_mult_pt(Up, view, up);
	Uplen = sqrt(Up[0] * Up[0] + Up[1] * Up[1]);
	if (Uplen == 0) return 0;
	M3d_make_z_rotation_cs(D, Up[1]/Uplen, Up[0]/Uplen);
	M3d_mat_mult(view, D, view);

	// now make the inverse

	M3d_make_translation(Ai, eye[0], eye[1], eye[2]);
	M3d_make_y_rotation_cs(Bi, c/p, a/p);
	M3d_make_x_rotation_cs(Ci, p/r, -b/r);
	M3d_make_z_rotation_cs(Di, Up[1]/Uplen, -Up[0]/Uplen);
	M3d_make_identity(view_inverse);
	M3d_mat_mult(view_inverse, Di, view_inverse);
	M3d_mat_mult(view_inverse, Ci, view_inverse);
	M3d_mat_mult(view_inverse, Bi, view_inverse);
	M3d_mat_mult(view_inverse, Ai, view_inverse);

	return 1;
}

double M3d_det_3x3(double M[3][3]) {
	double sum = 0;
	for (int i = 0; i < 3; i++) {
		double product_p = 1;
		double product_m = 1;
		for (int j = 0; j < 3; j++) {
			int r = j;
			int c = (i + j) % 3;
			double n = M[r][c];
			product_p *= n;
			r = (3 - j - 1) % 3;
			product_m *= M[r][c];
		}
		sum += product_p - product_m;
	}
	return sum;
}
