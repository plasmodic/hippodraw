/** @file

hippodraw::BinningProjector class implementation

Copyright (C) 2001-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinningProjector.cxx,v 1.123 2007/04/23 21:57:21 pfkeb Exp $

*/


#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "BinningProjector.h"

#include "axes/AxisModelBase.h"
#include "binners/BinsBase.h"
#include "datasrcs/NTuple.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::string;
using std::list;
using std::vector;
#endif

using namespace hippodraw;

BinningProjector::BinningProjector ( unsigned int axes )
  : m_binner_dim ( axes ),
    m_binner ( 0 )
{
}

BinningProjector::BinningProjector ( const BinningProjector & projector )
  : ProjectorBase ( projector ),
    m_binner_dim ( projector.m_binner_dim ),
    m_binner ( 0 )
{
  if ( projector.m_binner != 0 ) {
    m_binner = projector.m_binner->clone ();
    m_proj_values = m_binner -> createNTuple ();
    fillDataSource ( m_proj_values );
  } 
}

BinningProjector::~BinningProjector ()
{
  if ( m_binner != 0 ) {
    delete m_binner;
  }
}

void
BinningProjector::
setAxisModel ( hippodraw::Axes::Type axis, AxisModelBase * model )
{
  ProjectorBase::setAxisModel ( axis, model );
  checkScaling ();
}

void BinningProjector::prepareValues()
{
  bool is_dirty = isDirty();
  if ( is_dirty ) {
    execute();
    fillDataSource ( m_proj_values );
    setDirty ( false );
  }
}

const BinsBase *
BinningProjector::
getBinner ( ) const
{
  return m_binner;
}

void
BinningProjector::
setBinner ( BinsBase * bins )
{
  if ( m_binner != 0 ) delete m_binner;
  m_binner = bins;
  if ( m_proj_values != 0 ) delete m_proj_values;
  m_proj_values = m_binner -> createNTuple ();
}

void
BinningProjector::
setBinnerOn ( BinnerAxis * binner,
	      hippodraw::Axes::Type type )
{
  string axis;
  if ( type == Axes::X ) axis = "X";
  else if ( type == Axes::Y ) axis = "Y";
  else axis = "Z";

  bool yes = isAxisBinned ( axis );
  if ( ! yes ) return;

  m_binner->setBinnerOn ( binner, type );
  checkScaling ();

  setDirty ( true );
}

int
BinningProjector::
getNumberOfBins ( hippodraw::Axes::Type axis ) const
{
  assert ( m_binner );

  if ( axis == Axes::X || 
       ( m_binner_dim == 2 && axis == Axes::Y ) ) {
    return m_binner->numberOfBins ( axis );
  }

  return ProjectorBase::getNumberOfBins ( axis );
}

/** @todo Derived classes that re-implement this member function do so
    with only difference being the assert.  Redesigned to avoid
    duplication of code. 
*/
const Range &
BinningProjector::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  assert ( axis == Axes::X && width > 0. );

  const Range & range = m_binner->setBinWidth ( Axes::X, width );
  checkScaling ();

  setDirty ( true );

  return range;
}

const Range &
BinningProjector::
setBinWidth ( Axes::Type axis, 
	      int parm, 
	      bool dragging )
{
  double new_width = m_binner->calcBinWidth ( axis, parm, dragging );

  return setBinWidth ( axis, new_width );
}

void BinningProjector::setOffset ( const std::string & axis, 
				   int parm, 
				   bool dragging )
{
  if ( axis != "X" ) return;
  
  double new_offset = m_binner->calcOffset ( axis, parm, dragging );
  setOffset ( Axes::X, new_offset );

  setDirty ( true );
}

void
BinningProjector::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  if ( axis == Axes::Y ) return;

  m_binner->setOffset ( Axes::X, offset );
  m_x_axis->setRange( m_binner->getRange ( Axes::X ), true );

  setDirty ( true );
}


void BinningProjector:: reset ( )
{
}

double
BinningProjector::
getOffset ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ||
       ( axis == Axes::Y && m_binner_dim == 2 ) ) {
    return m_binner->getOffset ( axis );
  }

  // else Z
  return 0.0;
}

double
BinningProjector::
getBinWidth (hippodraw:: Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ||
       ( axis == Axes::Y && m_binner_dim == 2 ) ) {
    return m_binner->binWidth ( axis );
  }

  // else Z
  return 0.0;
}

/** @todo Is there away to avoid implementing this empty method. */
void BinningProjector::execute ()
{
}

void BinningProjector::checkScaling ()
{
  // does nothing
}

double BinningProjector::getZValue ( double x, double y ) const
{
  return m_binner->getZValue ( x, y );
}

DataSource *
BinningProjector::
createNTuple () const
{
  return m_binner->createNTuple ();
}

void
BinningProjector::
fillDataSource ( DataSource * ntuple, bool ) const
{
  m_binner -> fillDataSource ( ntuple );
}

void
BinningProjector::
normalizeTo ( double number )
{
  m_binner -> scaleNumberOfEntries ( number );
}

void
BinningProjector::
setNormalizing ( bool on )
{
  m_binner -> setEntriesScaling ( on );
}

/** @bug Only works if target projector is binning projector
 */
void
BinningProjector::
normalizeTo ( const ProjectorBase * target )
{
  if ( target != 0 ) {
    if ( target -> isValueBinned ( ) ) {
      m_target = target;
      ProjectorBase * t = const_cast < ProjectorBase * > ( target );
      t -> addObserver ( this );
      setNormalizing ( true );
      normalize ();
    }
  }
  else {
    if ( m_target != 0 ) {
      ProjectorBase * t = const_cast < ProjectorBase * > ( m_target );
      t -> removeObserver ( this );
      setNormalizing ( false );
    }
    m_target = 0;
  }
}

void
BinningProjector::
normalize ()
{
  int number = m_target ->getNumberOfEntries ();
  double norm = number;
  normalizeTo ( norm );

  setDirty ( true );
}

void
BinningProjector::
update ( const Observable * object )
{
  if ( object == m_target ) {
    normalize ();
  }

  else notifyObservers ();
}

void
BinningProjector::
willDelete ( const Observable * object )
{
  if ( object == m_target ) {
    m_target = 0;
    setNormalizing ( false );
    setDirty ( true );
  }
}

void
BinningProjector::
setBinContents ( const DataSource * source )
{
  m_binner -> setBinContents ( source );
}

void
BinningProjector::
setMinEntries ( int entries )
{
  m_binner -> setMinEntries ( entries );
  setDirty ( true );
}

int
BinningProjector::
getMinEntries ()
{
  return m_binner -> getMinEntries ();
}

bool
BinningProjector::
isImageConvertable () const
{
  return m_binner_dim == 2 &&
         m_binner -> hasEqualWidths();
}
