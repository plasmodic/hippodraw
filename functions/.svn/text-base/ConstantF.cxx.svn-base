/**@file

hippodraw::ConstantF class implementation

Copyright (C) 2000, 2001, 2003, 2005, 2007   The Board of Trustees of
The Leland Stanford Junior University.  All Rights Reserved.

$Id: ConstantF.cxx,v 1.37 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ConstantF.h"

#include "FunctionHelper.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
#endif

namespace hippodraw {

ConstantF::ConstantF ( )
{
  initialize ();
}

ConstantF::ConstantF ( double value )
{
  initialize ();

  m_parms[0] = value;
}

void ConstantF::initialize ()
{
  m_name = "Constant";
  m_parm_names.push_back ( "value" );

  resize ();
}

FunctionBase * ConstantF::clone () const
{
  return new ConstantF ( *this );
}

double ConstantF::operator () ( double ) const
{
  return m_parms[0];
}

void 
ConstantF::
initialParameters ( const FunctionHelper * helper )
{
  m_parms[0] = helper->meanValue ();
}

double ConstantF::derivByParm ( int i, double ) const
{
  assert ( i == 0 );

  return 1.0;
}

} // namespace hippodraw
