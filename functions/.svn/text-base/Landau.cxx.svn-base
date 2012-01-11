/**@file

hippodraw::Landau class implementation

Copyright (C) 2000-2003, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Landau.cxx,v 1.11 2007/09/30 15:57:09 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Landau.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>

using std::distance;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::exp;
using std::vector;
#endif

namespace hippodraw {

   static double p1[5] = {0.4259894875,-0.1249762550, 0.03984243700, -0.006298287635,   0.001511162253};
   static double q1[5] = {1.0         ,-0.3388260629, 0.09594393323, -0.01608042283,    0.003778942063};

   static double p2[5] = {0.1788541609, 0.1173957403, 0.01488850518, -0.001394989411,   0.0001283617211};
   static double q2[5] = {1.0         , 0.7428795082, 0.3153932961,   0.06694219548,    0.008790609714};

   static double p3[5] = {0.1788544503, 0.09359161662,0.006325387654, 0.00006611667319,-0.000002031049101};
   static double q3[5] = {1.0         , 0.6097809921, 0.2560616665,   0.04746722384,    0.006957301675};

   static double p4[5] = {0.9874054407, 118.6723273,  849.2794360,   -743.7792444,      427.0262186};
   static double q4[5] = {1.0         , 106.8615961,  337.6496214,    2016.712389,      1597.063511};

   static double p5[5] = {1.003675074,  167.5702434,  4789.711289,    21217.86767,     -22324.94910};
   static double q5[5] = {1.0         , 156.9424537,  3745.310488,    9834.698876,      66924.28357};

   static double p6[5] = {1.000827619,  664.9143136,  62972.92665,    475554.6998,     -5743609.109};
   static double q6[5] = {1.0         , 651.4101098,  56974.73333,    165917.4725,     -2815759.939};

   static double a1[3] = {0.04166666667,-0.01996527778, 0.02709538966};

   static double a2[2] = {-1.845568670,-4.284640743};


Landau::Landau ( )
{
  initialize ();
}

Landau::Landau ( double p, double c, double s )
{
  initialize ();
  
  m_parms[peak] = p;
  m_parms[norm] = c;
  m_parms[sigma] = s;
}

void Landau::initialize ()
{
  m_name = "Landau";

  m_parm_names.push_back ( "Peak" );
  m_parm_names.push_back ( "Normalization" );
  m_parm_names.push_back ( "Sigma" );

  resize ();
}

FunctionBase * Landau::clone () const
{
  return new Landau ( *this );
}

/** The original Fortran */
//                            REAL FUNCTION FITLAND(X)

//                             DOUBLE PRECISION FITEMP

//                             COMMON/PAWPAR/ PAR(3)


//                             PI=3.1415926

//                            Y=(X-PAR(1))/PAR(3)

//            FITEMP=DBLE(PAR(2)*EXP(-0.5*(Y+EXP(-1.*Y)))/SQRT(2.*PI))

//                               FITLAND=REAL(FITEMP)

//                            END
double Landau::operator () ( double x ) const
{  
  if(m_parms[sigma]==0) return 0;
  double v = calcY ( x );
   double u, ue, us, den;
   if (v < -5.5) {
      u   = exp(v+1.0);
      if (u < 1e-10) return 0.0;
      ue  = exp(-1/u);
      us  = sqrt(u);
      den = 0.3989422803*(ue/us)*(1+(a1[0]+(a1[1]+a1[2]*u)*u)*u);
   } else if(v < -1) {
      u   = exp(-v-1);
      den = exp(-u)*sqrt(u)*
             (p1[0]+(p1[1]+(p1[2]+(p1[3]+p1[4]*v)*v)*v)*v)/
             (q1[0]+(q1[1]+(q1[2]+(q1[3]+q1[4]*v)*v)*v)*v);
   } else if(v < 1) {
      den = (p2[0]+(p2[1]+(p2[2]+(p2[3]+p2[4]*v)*v)*v)*v)/
            (q2[0]+(q2[1]+(q2[2]+(q2[3]+q2[4]*v)*v)*v)*v);
   } else if(v < 5) {
      den = (p3[0]+(p3[1]+(p3[2]+(p3[3]+p3[4]*v)*v)*v)*v)/
            (q3[0]+(q3[1]+(q3[2]+(q3[3]+q3[4]*v)*v)*v)*v);
   } else if(v < 12) {
      u   = 1/v;
      den = u*u*(p4[0]+(p4[1]+(p4[2]+(p4[3]+p4[4]*u)*u)*u)*u)/
                (q4[0]+(q4[1]+(q4[2]+(q4[3]+q4[4]*u)*u)*u)*u);
   } else if(v < 50) {
      u   = 1/v;
      den = u*u*(p5[0]+(p5[1]+(p5[2]+(p5[3]+p5[4]*u)*u)*u)*u)/
                (q5[0]+(q5[1]+(q5[2]+(q5[3]+q5[4]*u)*u)*u)*u);
   } else if(v < 300) {
      u   = 1/v;
      den = u*u*(p6[0]+(p6[1]+(p6[2]+(p6[3]+p6[4]*u)*u)*u)*u)/
                (q6[0]+(q6[1]+(q6[2]+(q6[3]+q6[4]*u)*u)*u)*u);
   } else {
     u   = 1/(v-v*log(v)/(v+1));
      den = u*u*(1+(a2[0]+a2[1]*u)*u);
   }
   return m_parms[norm] * den/sigma;

   //Moyal old formula
  //  double t = exp ( -0.5 * ( y + exp ( -1.0 * y ) ) ) 
  //    / sqrt ( 2.0 * M_PI );
   //  return m_parms[norm] * t;
}

/* virtual */
void 
Landau::
initialParameters ( const FunctionHelper * helper )
{
  m_parms[norm] = helper->maxValue () * sqrt ( 2.0 * M_PI * M_E );
  m_parms[peak] = helper->meanCoord ();
  m_parms[sigma] = helper->stdCoord ();
}

// double Landau::derivByParm ( int i, double x ) const
// {
//   switch ( i ) {
//   case peak :
//     return derivByPeak ( x );
//     break;

//   case norm :
//     return derivByNorm ( x );
//     break;

//   case sigma :
//     return derivBySigma ( x );
//     break;

//   default :
//     assert ( false );
//     break;
//   }
//   return 0.0;
//}

double Landau::derivByNorm ( double x ) const
{
  double norm_aux = 0.0001;
  if(m_parms[norm] != 0) norm_aux = m_parms[norm];
  return operator () (x) / norm_aux;
}

double Landau::derivByPeak ( double x ) const
{
  return operator () ( x ) * calcZ ( x ) * ( ( -1.0 ) / m_parms[sigma] );
}

double Landau::derivBySigma ( double x ) const
{
  return operator () ( x ) * calcZ ( x ) 
    * ( - ( x - m_parms[peak] ) / ( m_parms[sigma] * m_parms[sigma] ) );
}

bool 
Landau::
hasDerivatives () const
{
  return false;
}


} // namespace hippodraw

