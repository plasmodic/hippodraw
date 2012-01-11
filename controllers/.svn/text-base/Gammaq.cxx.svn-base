/** @file

The Faithful implementation of the GammaQ.  

Code is taken from Numerical Recipes in C.


*/

#include "Gammaq.h"

#include <iostream>

#include <cassert>
#include <cmath>

using std::cerr;
using std::endl;
using std::abs;

namespace hippodraw {
  namespace Numeric {

double gammln( double xx )
{
  double x, y, tmp, ser;
  static double cof[6]={76.18009172947146,-86.50532032941677,
			24.01409824083091,-1.231739572450155,
			0.1208650973866179e-2,-0.5395239384953e-5};
  
  y = x = xx;
  tmp = x + 5.5;
  tmp -= ( x + 0.5 ) * log( tmp );
  ser = 1.000000000190015;
  
  for ( int j = 0; j <= 5; j++)
    ser += cof[j] / (++y);
  
  return -tmp + log( 2.5066282746310005 * ser / x);
}


void gser(double *gamser, double a, double x, double *gln)
{
  int n;
  double sum, del, ap;
  
  *gln = gammln( a );

  if ( x <= 0.0 )
    {
      if (x < 0.0) cerr << "Error: x less than 0 in routine gser" << endl;
      *gamser=0.0;
      return;
    }
  else
    {
      ap  = a;
      del = sum =1.0 / a;
      for (n = 1; n <= ITMAX; n++)
	{
	  ++ap;
	  del *= x/ap;
	  sum += del;
	  
	  if (abs(del) < abs(sum)*EPS)
	    {
	      *gamser = sum * exp( -x + a * log( x ) -( *gln ) );
	      return;
	    }
	}
      
      cerr << "ERROR: a too large, ITMAX too small in routine gser" << endl;
      return;
    }
}

void gcf( double *gammcf, double a, double x, double *gln )
{
  int i;
  double an,b,c,d,del,h;
  
  *gln = gammln( a );
  b = x + 1.0 - a;
  c = 1.0 / FPMIN;
  d = 1.0 / b;
  h = d;
  
  for( i = 1; i <= ITMAX; i++ )
    {
      an = -i*(i-a);
      b += 2.0;
      d=an*d+b;
      
      if ( abs( d ) < FPMIN )
	d = FPMIN;
      
      c = b + an / c;
      
      if ( abs( c ) < FPMIN )
	c = FPMIN;
      
      d   = 1.0 / d;
      del = d * c;
      h  *= del;
      
      if ( abs( del - 1.0 ) < EPS)
	break;
    }
  
  if (i > ITMAX) cerr << "a too large, ITMAX too small in gcf" << endl;
  
  *gammcf = exp( -x + a * log( x ) -( *gln ) ) * h;
}

double gammq(double a, double x)
{
  void gcf(double *gammcf, double a, double x, double *gln);
  
  double gamser,gammcf,gln;
  
  if (x < 0.0 || a <= 0.0)
    {
      cerr << "a = " << a << " x = " << x << endl;
      cerr << "Invalid arguments in routine GAMMQ" << endl;
      assert(0);
    }

  if (x < (a+1.0))
    {
      gser(&gamser,a,x,&gln);
      return 1.0-gamser;
    }
  else
    {
      gcf(&gammcf,a,x,&gln);
      return gammcf;
    }
}

} // end namespace Numeric
} // end namespace hippodraw

/** The main drive routine for testing the gammq function.
    It depends upon the existence if the file fncval.dat
    This routine also like above has been faithfully been
    copied from NRinC. Since it was copied from a C book
    you can see why printf and scanf's dominate the scene. */
    
/*
int main(void)
{
  char txt[MAXSTR];
  int i,nval;
  float a,val,x;
  FILE *fp;
  
  if ((fp = fopen("fncval.dat","r")) == NULL)
    printf("Data file fncval.dat not found\n");

  std::fgets(txt,MAXSTR,fp);
  
  while ( std::strncmp(txt,"Incomplete Gamma Function",25))
    {
      std::fgets(txt,MAXSTR,fp);
      if ( std::feof(fp))
	std::printf("Data not found in fncval.dat\n");
    }
  
  std::fscanf(fp,"%d %*s",&nval);
  std::printf("\n%s\n",txt);
  std::printf("%4s %11s %14s %14s \n","a","x","actual","gammq(a,x)");

  for (i=1;i<=nval;i++)
    {
      std::fscanf(fp,"%f %f %f",&a,&x,&val);
      std::printf("%6.2f %12.6f %12.6f %12.6f\n", a, x, (1.0-val), gammq(a,x));
    }
  
  std::fclose(fp);
  return 0;
}

*/

#undef ITMAX
#undef EPS
#undef FPMIN
#undef MAXSTR
