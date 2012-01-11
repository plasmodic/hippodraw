/** @file

hippodraw::FunctionBase class implementation

Copyright (C) 2000, 2003, 2005, 2007 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FunctionBase.cxx,v 1.69 2007/04/23 21:57:20 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "FunctionBase.h"

#include <cassert>

using std::string;
using std::vector;

namespace hippodraw {

/** Should never be called.. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. */
FunctionBase::FunctionBase ()
{
  m_name = "nil";
}

/** @request Remove this copy constructor, it was only tempoary to try
    to fix cloning functions written in Python with SIP.
*/
FunctionBase::
FunctionBase ( const FunctionBase & fb )
  : m_name ( fb.m_name ),
    m_parm_names ( fb.m_parm_names ),
    m_parms ( fb.m_parms )
{
}

FunctionBase::~FunctionBase ()
{
}

FunctionBase *
FunctionBase::
clone () const
{
  assert ( false );
  return 0;
}

void
FunctionBase::
initialParameters ( const FunctionHelper * )
{
}

void
FunctionBase::
setName ( const char * name )
{
  m_name = name;
}

void FunctionBase::resize ()
{
  assert ( ! m_parm_names.empty () );

  size_t size = m_parm_names.size ();
  m_parms.resize ( size );
//   m_fixed.resize ( size, false );

}

const string & FunctionBase::name () const
{
  return m_name;
}

const vector < string > & FunctionBase::parmNames ( ) const
{
  return m_parm_names;
}

void
FunctionBase::
setParmNames ( const std::vector < std::string > & names )
{
  m_parm_names = names;
  resize ();
}

const vector < double > & FunctionBase::getParameters () const
{
  return m_parms;
}

void
FunctionBase::
setParameters ( const std::vector< double > & incr )
{
  vector< double >::const_iterator it = incr.begin ();
  setParameters ( it );
}

vector < double > ::const_iterator
FunctionBase::
setParameters ( std::vector < double > :: const_iterator it )
{
  unsigned int size = m_parms.size();
  for ( unsigned int i = 0; i < size; i++ ) {
    m_parms[i] = *it++;
  }

  return it;
}

bool
FunctionBase::
hasDerivatives () const
{
  return true;
}

/** @request Override this method in derived classes when
    calculation of integral can be done analyically.
    
    @request Check if this is the best algorithm.  Its looks different
    from the one found in
    http://osl.iu.edu/%7Etveldhui/papers/techniques/
*/
double FunctionBase::integrate ( double a, double b ) const
{
  int n = 10;
  double h  = (b - a)/n;
  double x = a;
  double sumt = operator()( a ) / 2;
  
  for( int i = 1; i <=  n - 1; i++ )
    {
      x += h;
      sumt += operator()( x );
    }
  
  sumt = ( sumt + operator()( b ) / 2 ) * h;
  
  return sumt;
}

int FunctionBase::size () const
{
  return m_parm_names.size ();
}

bool FunctionBase::isComposite () const
{
  return false;
}

void FunctionBase::addToComposite ( FunctionBase * )
{
}

void FunctionBase::removeFromComposite ( FunctionBase * )
{
}

int FunctionBase::count()
{
  return 0;
}

double
FunctionBase::
operator () ( const std::vector < double > & v ) const
{
  assert ( v.size () == 1 );

  return this -> operator () ( v.front () );
}

double
FunctionBase::
derivByParm ( int, double ) const
{
	assert ( false );
	return 0.;
}

void
FunctionBase::
initialize ()
{
	assert ( false );
}

double
FunctionBase::
operator () ( double ) const
{
	assert ( false );
	return 0.;
}

unsigned int
FunctionBase::
dimensions () const
{
  return 1;
}

} // namespace hippodraw

