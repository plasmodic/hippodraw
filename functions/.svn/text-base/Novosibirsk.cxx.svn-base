/** @file

hippodraw::Novosibirsk class implementation

Copyright (C) 2005 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Novosibirsk.cxx,v 1.9 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "Novosibirsk.h"

#include "FunctionHelper.h"

#include <cmath>
#include <cassert>

using std::exp;
using std::vector;

using namespace hippodraw;

Novosibirsk::Novosibirsk ( )
{
  initialize ();
}

Novosibirsk::Novosibirsk ( double n, double m, double s, double t )
{
  initialize ();
  
  m_parms[norm] = n;
  m_parms[mean] = m;
  m_parms[sigma] = s;
  m_parms[tail] = t;
}

void Novosibirsk::initialize ()
{
  m_name = "Novosibirsk";

  m_parm_names.push_back ( "Norm" );
  m_parm_names.push_back ( "Mean" );
  m_parm_names.push_back ( "Sigma" );
  m_parm_names.push_back ( "Tail" );

  resize ();
}

FunctionBase * Novosibirsk::clone () const
{
  return new Novosibirsk ( *this );
}

double Novosibirsk::operator () ( double x ) const
{
//---- If tail is small then Gauss

  double  qa=0,qb=0,qc=0,qx=0,qy=0;
  double result=0;

  if(fabs(m_parms[tail]) < 1.e-7)
    qc = 0.5*pow((( x -m_parms[mean])/m_parms[sigma]),2);
  else {
    qa = m_parms[tail]*sqrt(log(4.));
    qb = sinh(qa)/qa;
    qx = ( x - m_parms[mean])/m_parms[sigma]*qb;
    qy = 1.+m_parms[tail]*qx;

    //---- Cutting curve from right side

    if( qy > 1.E-7)
      qc = 0.5*(pow((log(qy)/m_parms[tail]),2) + m_parms[tail]*m_parms[tail]);
    else
      qc = 15.;
  }
  //----

  result =  m_parms[norm] * exp(-qc);

  return result;
}

void 
Novosibirsk::
initialParameters ( const FunctionHelper * helper )
{
  double min_x = helper->minCoord ();
  double max_x = helper->maxCoord ();
  int size = helper->size();
  double total = helper->getTotal ();

  m_parms[norm] = total * ( max_x - min_x ) / size;
  m_parms[mean] = helper->meanCoord ();
  m_parms[sigma] = helper->stdCoord ();
  m_parms[tail] = 0.;
}

double
Novosibirsk::
derivByParm ( int , double ) const
{
  assert ( false );
  return 0.;
}

bool
Novosibirsk::
hasDerivatives () const
{
  return false;
}
