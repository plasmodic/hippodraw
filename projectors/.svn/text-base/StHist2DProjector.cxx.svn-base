/** @file

hippodraw::StHist2DProjector class implementation

Copyright (C) 2003-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: StHist2DProjector.cxx,v 1.279 2007/04/23 21:57:21 pfkeb Exp $

*/

#ifdef _MSC_VER
// Include max() and min() missing from MicroSoft Visual C++.
#include "msdevstudio/MSconfig.h"
#endif

#include "StHist2DProjector.h"

#include "axes/AxisModelBase.h"

#include "binners/BinsBase.h"
#include "binners/BinsFactory.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"

#include <numeric>

#include <cassert>

using namespace hippodraw;

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::accumulate;
using std::inner_product;
using std::list;
using std::max;
using std::string;
using std::vector;
#endif

StHist2DProjector::StHist2DProjector( )
  : Hist2DProjImp ( )
{
  m_z_label = "Entries / bin";
}

/** @todo Extra data members need to be copied. 
    @note Must initialize with the copy constructor of ProjectorBase
    here, otherise the default constructor will be used.  This is
    because it is a virtual base class of the inherited classes.
 */
StHist2DProjector::
StHist2DProjector ( const StHist2DProjector & projector )
  : ProjectorBase ( projector ),
    Hist2DProjImp ( projector ),
    m_title ( projector.m_title ),
    m_x_label ( projector.m_x_label ),
    m_y_label ( projector.m_y_label ),
    m_z_label ( projector.m_y_label )
{
}

ProjectorBase * StHist2DProjector::clone()
{
  return new StHist2DProjector ( *this );
}

/** @todo This method could have common implementation with
    DyHist2DProjector if the static projector initialized it binding
    options even thought they do not bind to NTuple.
 */
bool StHist2DProjector::isAxisBinned ( const std::string & axis ) const
{
  return axis == "x" || 
    axis == "X" ||
    axis == "y" ||
    axis == "Y";
}

double
StHist2DProjector::
getPosOn( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  return getPosOnValue ();
}

Range
StHist2DProjector::
dataRangeOn (hippodraw:: Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    return m_binner->getRange ( axis );
  }
  if ( axis == Axes::Y ) {
    return m_binner->getRange ( axis );
  }

  return dataRangeOnValue ();
}

const string & StHist2DProjector::getXLabel() const
{
  return m_x_label;
}

const string & StHist2DProjector::getYLabel ( bool ) const
{
  return m_y_label;
}

const string & StHist2DProjector::getZLabel ( bool ) const
{
 return m_z_label;
}

namespace dp = hippodraw::DataPoint3DTuple;

double
StHist2DProjector::
getAverage ( hippodraw::Axes::Type axis ) const
{
  StHist2DProjector * p = const_cast < StHist2DProjector * > ( this );
  p -> prepareValues ();

  unsigned int col = 3;    // bad value
  switch ( axis ) {
      
  case Axes::X:
    col = dp::X;
    break;
    
  case Axes::Y:
    col = dp::Y;
      break;
    
  case Axes::Z:
    col = dp::Z;
    break;

  default:
    break;
  }
  assert ( col < 3 );

  double result = 0.0;
  const DataSource * ntuple = getProjectedValues ();
  const vector < double > & value = ntuple -> getColumn ( dp::Z );

  if ( col < 2 ) {
    const vector < double > & data = ntuple -> getColumn ( col );

    double sumXV = 0.0;
    // Doxygen 1.5.0 needs the std::
    sumXV = std::inner_product ( data.begin(), data.end(),
				 value.begin(), sumXV );

    double sumV = 0.0;
    sumV = accumulate ( value.begin(), value.end(), sumV );

    result =  sumXV / sumV;
  }
  else {
	double sumV = 0.0;
    sumV = accumulate ( value.begin(), value.end (), sumV );
    result = ( sumV / value.size () ) * m_z_axis -> getScaleFactor ();
  }
  p -> setDirty ( true );

  return result;
}

/* virtual */
const std::string & StHist2DProjector::getTitle () const
{
  return m_title;
}

int
StHist2DProjector::
getNumberOfEntries () const
{
  double sum = m_binner->getNumberOfEntries ();

  return static_cast < int > ( sum );
}

int
StHist2DProjector::
getUnderflow () const
{
  return m_binner->getUnderflow ();
}

int 
StHist2DProjector::
getOverflow () const
{
  return m_binner->getOverflow ();
}

void
StHist2DProjector::
addValues ( const std::vector < double > & v )
{
  double x = v[0];
  double y = v[1];
  double w = v.size() == 3 ? v[2] : 1.0;

  m_binner -> accumulate ( x, y, w );
  setDirty ();

  notifyObservers ();
}

bool
StHist2DProjector::
isEmpty () const
{
  return false;
}

void
StHist2DProjector::
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
