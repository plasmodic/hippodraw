/** @file

hippodraw::LinearSumFunction class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: LinearSumFunction.cxx,v 1.54 2007/04/23 21:57:20 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "LinearSumFunction.h"

#include "pattern/string_convert.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::vector;
using std::string;
#endif

using namespace hippodraw;

LinearSumFunction::LinearSumFunction ( )
{
  initialize ();
}

LinearSumFunction::
LinearSumFunction ( const LinearSumFunction & old )
  : FunctionBase ()
{
  FunctionList_t::const_iterator it = old.m_functions.begin();
  while ( it != old.m_functions.end() ) {
    FunctionBase * function = (*it)->clone ();
    m_functions.push_back ( function );
  }

  initialize ();
}

void LinearSumFunction::initialize ()
{
  m_name = "Linear Sum";
}

FunctionBase * LinearSumFunction::clone () const
{

  return new LinearSumFunction ( *this );
}

const vector < string > & LinearSumFunction::parmNames() const
{
  LinearSumFunction * self = const_cast < LinearSumFunction * > ( this );

  self->m_parm_names.clear();

  unsigned int f_size = m_functions.size ();
  for ( unsigned int i = 0; i < f_size; i++ ) {
    string suffix ( "-" );
    suffix += String::convert ( i );

    const vector< string > & names = m_functions[i] -> parmNames ();
    unsigned int n_size = names.size ();
    for ( unsigned int j = 0; j < n_size; j++ ) {
      string name = names [j];
      name += suffix;
      self -> m_parm_names.push_back ( name );
    }
  }

  return m_parm_names;
}

const vector<double> & LinearSumFunction::getParameters () const
{
  LinearSumFunction  * p = const_cast < LinearSumFunction * > ( this );
  p->m_parms.clear ();
  FunctionList_t::const_iterator it = m_functions.begin ();
  for ( ; it != m_functions.end (); ++it ) {
    const vector<double> & vals = (*it)->getParameters ();
    p->m_parms.insert ( p->m_parms.end (), vals.begin(), vals.end () ); 
  }
  return m_parms;
}


vector< double >::const_iterator
LinearSumFunction::
setParameters ( std::vector< double >::const_iterator it )
{
  FunctionList_t::iterator fit = m_functions.begin();

  for ( ;fit != m_functions.end (); ++fit ) {
    it = (*fit)->setParameters ( it );
  }

  return it;
}


double LinearSumFunction::derivByParm ( int index, double x ) const
{
  double value = 0;
  unsigned int numf = m_functions.size ();
  for ( unsigned int i = 0; i < numf; i++ ) {
    int size = m_functions [i] -> size ();
    if ( index < size ) {
      value = m_functions [i] -> derivByParm ( index, x );
      break;
    }
    else {
      index -= size;
    }
  }

  return value;
}

int LinearSumFunction::count ()
{
  return m_functions.size();
}

int
LinearSumFunction::
size () const
{
  int number = 0;
  unsigned int numf = m_functions.size ();

  for ( unsigned int i = 0; i < numf; i++ ) {
    const FunctionBase * function = m_functions [i];
    number += function -> size ();
  }

  return number;
}

bool LinearSumFunction::isComposite () const
{
  return true;
}

void LinearSumFunction:: addToComposite ( FunctionBase * function )
{
  m_functions.push_back ( function );
}

void
LinearSumFunction::
removeFromComposite ( FunctionBase * function )
{
  FunctionList_t::iterator it = m_functions.begin ();

  while ( it != m_functions.end () ) {
    if ( (*it) == function ){
      it = m_functions.erase ( it );
    }
    else{
      it ++;
    }
  }
}

double LinearSumFunction::operator () ( double x ) const
{
  double sum = 0.0;
  FunctionList_t::const_iterator it = m_functions.begin ();

  for ( ; it != m_functions.end (); ++it ) {
    sum += (*it)->operator () ( x );
  }
  return sum;
}

void 
LinearSumFunction::
initialParameters ( const FunctionHelper * )
{
  // does nothing
}
