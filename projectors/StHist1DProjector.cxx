/** @file

hippodraw::StHist1DProjector class implementation

Copyright (C) 2003, 2004, 2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: StHist1DProjector.cxx,v 1.273 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "StHist1DProjector.h"

#include "ProjectorHelper.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::max;
using std::string;
using std::vector;
#endif

StHist1DProjector::StHist1DProjector( )
  : Hist1DProjImp ( )
{
  m_y_label = "Entries / bin";
}

/** @todo Extra data members need to be copied. 
    @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
*/
StHist1DProjector::
StHist1DProjector ( const StHist1DProjector & projector )
  : ProjectorBase ( projector ),
    Hist1DProjImp ( projector ),
    m_title ( projector.m_title ),
    m_x_label ( projector.m_x_label ),
    m_y_label ( projector.m_y_label )
{
}

ProjectorBase * StHist1DProjector::clone()
{
  return new StHist1DProjector ( *this );
}

/* virtual */
bool StHist1DProjector::isAxisBinned ( const std::string & axis ) const
{
  return axis == "x" || axis == "X";
}

/** @todo Return positive value from the binner.
    @todo Redesign to avoid duplicated code with Hist1DProjector.
*/
double
StHist1DProjector::
getPosOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  return getPosOnValue ();
}

Range
StHist1DProjector::
dataRangeOn ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );

  if ( axis == Axes::X ) {
    return m_binner->getRange ( axis );
  }

  // Y
  return dataRangeOnValue ();
}

const string & StHist1DProjector::getXLabel() const
{
  return m_x_label;
}

void StHist1DProjector::setYLabel()
{
  m_y_label = m_y_label_entries;
}

const string & StHist1DProjector::getYLabel ( bool ) const
{
 return m_y_label;
}

double
StHist1DProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  StHist1DProjector * p = const_cast < StHist1DProjector * > ( this );
  p -> prepareValues (); // might yet be done.

  ProjectorHelper helper( getProjectedValues () );
  
  double mean = 0.0;
  
  if ( axis == Axes::X ) {
    mean = helper.meanCoord();
  }
  else if ( axis == Axes::Y ) {
    mean = helper.meanValue() * m_y_axis->getScaleFactor();
  }
  p -> setDirty ( true );   
  return mean;
}

/* virtual */
const std::string & StHist1DProjector::getTitle () const
{
  return m_title;
}

int
StHist1DProjector::
getNumberOfEntries () const
{
  double sum = m_binner->getNumberOfEntries ();

  return static_cast < int > ( sum );
}

int
StHist1DProjector::
getUnderflow () const
{
  int underflow = m_binner->getUnderflow ();
  return underflow;
}

int
StHist1DProjector::
getOverflow () const
{
  int overflow = m_binner->getOverflow ();
  return overflow;
}
  

void
StHist1DProjector::
addValues ( const std::vector < double > & v )
{
  double x = v[0];
  double w = v.size() == 2 ? v[1] : 1.0;

  m_binner -> accumulate ( x, w );
  setDirty ();

  notifyObservers ();
}

bool
StHist1DProjector::
isEmpty () const
{
  return false;
}

void
StHist1DProjector::
setBinnerRange ( hippodraw::Axes::Type axis,
		 const Range & range,
		 bool const_width )
{
  if ( m_binner -> isEmpty () ) {
    m_binner -> setRange ( axis, range, const_width );
    checkScaling ();
  }
  setDirty ( true );
}

void
StHist1DProjector::
reset ()
{
  m_binner -> reset ();
  setDirty( true );
}
    
