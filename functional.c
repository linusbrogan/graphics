#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double sum (double (*f)(double x),
	    double xlo,
	    double xhi,
	    double step)
{
  double x,s ;

  s = 0 ;
  for (x = xlo ; x <= xhi ; x += step) {
    s = s + f(x) ;
  }

  return s ;
}



int main()
{
  double v ;

  v = sum(sqrt, 10,15,1) ;
  printf("%lf\n", v) ;

  v = sum(sin, 0,6,2) ;
  printf("%lf\n", v) ;  
}
