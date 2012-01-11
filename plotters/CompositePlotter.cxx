/** @file

hippodraw::CompositePlotter class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CompositePlotter.cxx,v 1.301 2007/09/13 18:53:23 pfkeb Exp $

*/


#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "CompositePlotter.h"

#include "PlotterException.h"

#include "axes/AxisModelLinear.h"
#include "datareps/DataRep.h"

#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "graphics/Rectangle.h"
#include "projectors/ProjectorBase.h"

#include "reps/AxisRepBase.h"
#include "reps/ColorBoxPointRep.h"

#include "transforms/PeriodicBinaryTransform.h"
#include "transforms/TransformFactory.h"
#include "transforms/XYZTransform.h"

#include <algorithm>
#include <functional>

#include <cmath>
#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::mem_fun;
using std::string;
using std::vector;
using std::find;
#endif

using namespace hippodraw;

CompositePlotter::
CompositePlotter ( )
  : m_x_axis ( 0 ),
    m_y_axis ( 0 ),
    m_z_axis ( 0 ),
    m_x_label( "%x" ), 
    m_y_label( "%y" ),
    m_z_label( "%z" ),
    m_transform ( 0 ),
    m_fits_transform ( 0 ),
    m_datarep_index ( -1 ),
    m_has_autoscaled ( false ),
    m_show_grid ( false ),
    m_box_edge (false),
    m_has_z ( false ),
    m_reverse ( false )
    
{
  m_x_axis = new AxisModelLinear ( PLOTBOTTOM, PLOTBOTTOM );
  m_y_axis = new AxisModelLinear ( PLOTLEFT,  PLOTLEFT );
  m_z_axis = 0;

  TransformFactory * factory = TransformFactory::instance ();
  m_transform = factory->createTransform ( "Linear Linear" );
}

CompositePlotter::CompositePlotter ( const CompositePlotter & plotter )
  : m_x_label( plotter.m_x_label ),
    m_y_label( plotter.m_y_label ),
    m_z_label( plotter.m_z_label ),
    m_datarep_index ( plotter.m_datarep_index ),
    m_has_autoscaled ( plotter.m_has_autoscaled ),
    m_show_grid ( plotter.m_show_grid ),
    m_box_edge ( plotter.m_box_edge),
    m_has_z ( plotter.m_has_z ),
    m_reverse ( plotter.m_reverse )

{
  m_x_axis = plotter.m_x_axis ? plotter.m_x_axis->clone () : 0;
  m_y_axis = plotter.m_y_axis ? plotter.m_y_axis->clone () : 0;
  m_z_axis = plotter.m_z_axis ? plotter.m_z_axis->clone () : 0;

  if ( m_has_z ) setEnableZ ( true );

  if ( plotter.m_transform != 0 ) {
    m_transform = plotter.m_transform->clone ();
  }

  if ( plotter.m_fits_transform != 0 ) {
    m_fits_transform = plotter.m_fits_transform->clone ();
  } else {
    m_fits_transform = 0;
  }

  const vector < DataRep * > & datareps = plotter.m_datareps;
  vector< DataRep * >::const_iterator first = datareps.begin ();

  for ( ; first != datareps.end (); ++first ) {
    DataRep * datarep = (*first)->clone ();
    m_datareps.push_back ( datarep );
  }

}

CompositePlotter::~CompositePlotter ()
{
  delete m_y_axis;
  if ( m_z_axis != 0 ) delete m_z_axis;

  if ( m_transform != 0 ) delete m_transform;

  if ( m_fits_transform != 0 ) delete m_fits_transform;

  vector < DataRep * > :: iterator first = m_datareps.begin();
  while ( first != m_datareps.end() ) {
    delete *first++;
  }
}

CompositePlotter *
CompositePlotter::
clone ()
{
  return new CompositePlotter ( *this );
}

hippodraw::DataRep * CompositePlotter::selectedDataRep () const
{
  int index = m_datarep_index < 0 ? 0 : m_datarep_index;

  return m_datareps[index];
}

bool CompositePlotter::hasNTupleBindings () const
{
  DataRep * rep = selectedDataRep ();
  if ( rep == 0 ) return false;

  return rep->hasNTupleBindings ();
}

bool CompositePlotter::hasZoomY () const
{
  bool retVal = true;
  
  for ( unsigned int i = 0; i < m_datareps.size () ; i++ )
    {
      retVal = retVal &= ( m_datareps[i] -> hasZoomY () ); 
    }
  return retVal != 0;

}

int
CompositePlotter::
setActivePlot ( int index, bool )
{
  int retval = -1;
  m_datarep_index = index;

  vector< DataRep * >::iterator it = m_datareps.begin();

  if ( index < 0 ) { // set all or none to be selected.
    bool yes = index == -1;
    for ( ; it != m_datareps.end(); ++it ) {
      (*it)->setSelected ( yes );
    }
    retval = index;
    checkAxisScaling ();
  } 
  else {
    it = m_datareps.begin();
    for ( int i = 0; it != m_datareps.end(); ++it, ++i ) {
      DataRep * rep = *it;
      if ( i == index ) {
	rep->setSelected ( true );
	ProjectorBase * projector = rep -> getProjector ();
	projector -> checkScaling ();
      }
      else {
	rep->setSelected ( false );
      }
    }
    retval = m_datarep_index;
  }

  return retval;
}

int CompositePlotter::activePlotIndex () const
{
  return m_datarep_index;
}

void CompositePlotter::push_back ( DataRep * rep )
{
  vector < DataRep * > :: iterator first 
    = find ( m_datareps.begin (), m_datareps.end(), rep );
  if ( first != m_datareps.end () ) return;

  m_datareps.push_back ( rep );
}

void CompositePlotter::addDataRep ( DataRep * rep )
{
  push_back ( rep );

  if ( m_datareps.size() == 1 ) {
    setActivePlot ( 0, false );
  }
  else {
    setActivePlot ( -1, false );
  }

  assert ( m_x_axis );
  assert ( m_y_axis );

  rep->setAxisModel ( Axes::X, m_x_axis );
  rep->setAxisModel ( Axes::Y, m_y_axis );

  // I think this is smarter than putting this whole function in
  // derived classes.
  if ( hasAxis ( Axes::Z ) ) rep->setAxisModel ( Axes::Z, m_z_axis );

  checkAxisScaling ();
}

ProjectorBase * CompositePlotter::activeProjector () const
{
  DataRep * active_datarep = 0;

  if ( m_datarep_index < 0 ) {
    active_datarep = m_datareps.front();
  }
  else {
    active_datarep = m_datareps[m_datarep_index];
  }

  return active_datarep->getProjector ();
}

ProjectorBase * CompositePlotter::getProjector ( int index ) const
{
  assert( index < getNumDataReps() );

  DataRep * datarep = m_datareps[index];

  return datarep->getProjector();
}

int CompositePlotter::getNumDataReps() const
{
  int i = static_cast< int >( m_datareps.size() );

  return i;
}

/* virtual */
DataRep * CompositePlotter::getDataRep ( int index ) const
{
  if ( index < 0 ) return 0;
  if ( index < getNumDataReps () ) return m_datareps[index];
  // else
  return 0;
}

DataRep * CompositePlotter::getParentDataRep ( int index ) const
{
  DataRep * drep = getDataRep( index );
  if( drep  != 0 )
    return drep -> getParentDataRep();
  else
    return 0;

  return 0;
}

DataRep * CompositePlotter::getParentDataRep () const
{
  DataRep * drep = getDataRep( m_datarep_index );
  if( drep  != 0 )
    return drep -> getParentDataRep();
  else
    return 0;
  
  return 0;
}

void CompositePlotter::setParentDataRep ( int index, DataRep * parent )
{
  DataRep * drep = getDataRep( index );

  assert( drep );
  
  drep -> setParentDataRep( parent );
}

void CompositePlotter::setParentDataRep ( DataRep * parent )
{
  DataRep * drep = getDataRep(  m_datarep_index );

  assert( drep );
  
  drep -> setParentDataRep( parent );
}


void CompositePlotter::removeDataRep ( DataRep * rep )
{ 
  
  vector < DataRep * >::iterator it 
    = find ( m_datareps.begin(), m_datareps.end(), rep );
  if ( it == m_datareps.end () ) {
    return;
  }

  m_datareps.erase ( it );

  if ( getNumDataReps() == 1 ) m_datarep_index = 0;
  if ( m_datarep_index >= getNumDataReps() ) m_datarep_index = 0;

  checkAxisScaling ();
}

void CompositePlotter::setAllAxisModels ()
{
  vector < DataRep * >::iterator first = m_datareps.begin ();
  for ( ; first != m_datareps.end (); ++first ) {
    (*first)->setAxisModel ( Axes::X, m_x_axis );
    (*first)->setAxisModel ( Axes::Y, m_y_axis );
    (*first)->setAxisModel ( Axes::Z, m_z_axis );
  }
}

void
CompositePlotter::
autoScale ( AxisModelBase * model, hippodraw::Axes::Type axis )
{
  if ( model->isAutoRanging () == false ) return;

  BinaryTransform * transform 
    = dynamic_cast < BinaryTransform * > ( m_transform );

  bool all_empty = true;
  vector< DataRep * >::iterator it = m_datareps.begin();

  while ( it != m_datareps.end () ) {
    DataRep * rep = *it++;
    if ( rep->hasZeroRows() ) continue;
    all_empty = false;
    Range range = rep->preferredRange ( axis );
    model->setUnionRange ( range );
  }
  if ( all_empty == true ) return;

  if ( axis == Axes::X ) {
    const Range & range = transform->limitX ();
    transform->adjustValues ( *model, axis, range );
  }
  else if ( axis == Axes::Y ) {
    const Range & range = transform->limitY ();
    transform->adjustValues ( *model, axis, range );
  }

  it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    DataRep * rep = *it++;
    rep->setRange ( axis, false );
  }
}

void
CompositePlotter::
autoScale ( hippodraw::Axes::Type axis )
{
  switch ( axis )
    {
    case Axes::X :
      m_x_axis -> setEmpty ();
      autoScale ( m_x_axis, axis );
      break;
    case Axes::Y :
      m_y_axis -> setEmpty ();
      autoScale ( m_y_axis, axis );
      break;
    case Axes::Z :
      autoScaleZ ();
      break;
    default :
      break;
    }
}

void CompositePlotter::autoScale ( )
{
  bool z_auto = m_z_axis != 0 ? m_z_axis->isAutoRanging () : false;

  m_x_axis -> setEmpty ();
  m_y_axis -> setEmpty ();
  autoScale ( m_x_axis, Axes::X );
  autoScale ( m_y_axis, Axes::Y );

  // And finally Z axis
  if ( z_auto ) {
    autoScaleZ ();
  }

  setAutoScaled ( true );
}

void
CompositePlotter::
autoScaleZ ()
{
  m_z_axis->setEmpty ();

  vector< DataRep * >::iterator it = m_datareps.begin();
  while ( it != m_datareps.end () ) {
    DataRep * rep = *it++;
    if ( rep->hasZeroRows() ) continue;
    
    if ( rep -> hasAxis ( Axes::Z ) == true ) {
      Range range = rep->preferredRange ( Axes::Z );
      m_z_axis->setUnionRange ( range );
    }
  }

  BinaryTransform * transform 
    = dynamic_cast < BinaryTransform * > ( m_transform );
  Range range = transform->limitZ ();
  const Range & cur_range = m_z_axis -> getRange ( false );
  double pos = cur_range.pos();
  range.setPos ( pos );
  m_z_axis -> setIntersectRange ( cur_range, range );
  it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    DataRep * rep = *it++;
    if ( rep -> hasAxis ( Axes::Z ) ) {
      rep->setRange ( Axes::Z, false );
    }
  }

  // Set the low edge of the range of Z axis.
  // Used for colorbox and contour plots.
  if (m_z_axis->isLog()) {
    double step=pow(cur_range.high()/cur_range.pos(), 0.05);
    double low = cur_range.pos()/step;
    m_z_axis->setRange ( low, cur_range.high(), low );
  }
}

bool CompositePlotter::isDirty () const
{
  bool yes = false;
  vector < DataRep * >:: const_iterator first = m_datareps.begin();
  for ( ; first != m_datareps.end(); ++ first ) {
    yes |= (*first)->isDirty();
  }
  return yes;
}


void CompositePlotter::drawProjValues ( ViewBase * view )
{
  DataRep * active_datarep = 0;
  if ( m_datarep_index < 0 )  {
    unsigned int size = m_datareps.size ();

    for ( unsigned int i = 0; i < size; i++ ) {
      DataRep * rep = m_datareps[i];
      if ( rep -> hasCut () ) {
      
	toggleBoxEdge(rep);       
	rep -> drawProjectedValues ( m_transform, view );
      }
    }

    for ( unsigned int i = 0; i < size; i++ ) {
      DataRep * rep = m_datareps[i];
      if ( rep -> hasCut () == false ) {
  
	toggleBoxEdge(rep); 
	rep -> drawProjectedValues ( m_transform, view );
      }
    }
  }
  else {
    if ( m_datareps.empty () ) {
      return;
    }
    assert ( m_datarep_index < m_datareps.size() );

//     active_datarep = m_datareps[m_datarep_index];
    active_datarep = m_datareps.at(m_datarep_index);

    vector< DataRep * >::iterator it = m_datareps.begin();

    for ( ; it != m_datareps.end(); ++it ) {
      if ( *it != active_datarep ) {
	toggleBoxEdge(*it); 
	(*it)->drawProjectedValues ( m_transform, view );
      }
    }
    toggleBoxEdge(active_datarep); 
    active_datarep->drawProjectedValues ( m_transform, view );
  }
}

void
CompositePlotter::
setRange ( hippodraw::Axes::Type axis, const Range & range,
	   bool scaled, bool adjust_width )
{
  setRangePrivate ( axis, range, scaled, adjust_width );
  vector< DataRep * >::iterator it = m_datareps.begin();
  bool yes = adjust_width == false;
  for ( ; it != m_datareps.end(); ++it ) {
    (*it)->setRange ( axis, yes );
  }
}

double
CompositePlotter::
getPosRange ( hippodraw::Axes::Type axis ) const
{
  double min_pos = DBL_MAX;
  vector< DataRep * >::const_iterator it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    DataRep * rep = *it++;
    if ( rep -> hasAxis ( axis ) ) {
      double pos = rep -> getPosRange ( axis );
      if ( pos > 0.0 ) min_pos = std::min ( min_pos, pos );
    }
  }

  return min_pos;
}

void
CompositePlotter::
setNumberOfBins ( hippodraw::Axes::Type axis, unsigned int number )
{
  vector < DataRep * >:: iterator it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    ProjectorBase * projector = (*it++)->getProjector ();
    projector->setNumberOfBins ( axis, number );
  }
}

void
CompositePlotter::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  vector < DataRep * >:: iterator it = m_datareps.begin();

  while ( it != m_datareps.end() ) {
    ProjectorBase * projector = (*it++)->getProjector ();
    projector->setBinWidth ( axis, width );
  }
}

void
CompositePlotter::
reset ()
{
  vector < DataRep * >:: iterator it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    ProjectorBase * projector = (*it++)->getProjector ();
    projector->reset ( );
  }
}

void
CompositePlotter::
matrixTranspose ( bool yes )
{
  for_each ( m_datareps.begin(), m_datareps.end(),
	     bind2nd ( mem_fun ( &DataRep::matrixTranspose ), yes ) );
}

void
CompositePlotter::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  vector < DataRep * >:: iterator it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    ProjectorBase * projector = (*it++)->getProjector ();
    projector->setOffset ( axis, offset );
  }
}

void
CompositePlotter::
setErrorDisplay ( hippodraw::Axes::Type axis, bool flag )
{
  if ( m_datarep_index < 0 ) return;

  DataRep * datarep = m_datareps[m_datarep_index];
  datarep->setErrorDisplay ( axis, flag );
}

bool
CompositePlotter::
errorDisplay ( hippodraw::Axes::Type axis ) const
{
  DataRep * datarep = m_datareps.front ();

  return datarep->isErrorDisplayed ( axis );
}

void CompositePlotter::setRepresentation ( RepBase * pointrep )
{
  if ( m_datarep_index < 0 ) return;

  DataRep * datarep = m_datareps[m_datarep_index];
  datarep->setPointRep ( pointrep );
}

RepBase * CompositePlotter::representation ( ) const
{
  DataRep * datarep = 0;
  if ( m_datarep_index < 0 ) {
    datarep = m_datareps.front();
  }
  else {
  datarep = m_datareps[m_datarep_index];
  }

  return datarep->getRepresentation ();
}

const BinToColor *
CompositePlotter::
getValueRep () const
{
  RepBase * rep = representation ();

  return rep -> getValueTransform ();
}



void
CompositePlotter::
setValueRep ( BinToColor * btc )
{
  RepBase * rep = representation ();
  rep -> setValueTransform ( btc );
}

int
CompositePlotter::
getNumberOfEntries () const
{
  int index = activePlotIndex();
  int number = 0;

  if ( ! ( index < 0 ) ) {
    const DataRep * rep = getDataRep ( index );
    number = rep -> getNumberOfEntries ();
  }
  return number;
}

/* virtual */
double
CompositePlotter::
getBinWidth ( hippodraw::Axes::Type axis ) const
{
  int index = activePlotIndex ();

  if ( !( index < 0 ) ) {
    ProjectorBase * projector = getProjector ( index );
    return projector->getBinWidth ( axis );
  }

  vector< DataRep * >::const_iterator it = m_datareps.begin();

  string saxis;
  if ( axis == Axes::X ) {
    saxis = "X";
  } else if ( axis == Axes::Y ) {
    saxis = "Y";
  } else {
    saxis = "Z";
  }

  double first = -1.0;
  for ( ; it != m_datareps.end(); ++it ) {

    ProjectorBase * projector = (*it)->getProjector();

    if ( !( projector->isAxisBinned ( saxis ) ) ) {
      continue;
    }

    if ( first < 0.0 ) {
      first = projector->getBinWidth ( axis );
      continue;
    }

    double next = projector->getBinWidth ( axis );

    // Be careful here.  first != next doesn't work if compiler doesn't
    // store next to member because of optimization.  On Intel
    // platform, register has different size than memory, so
    // comparison could fail.
    if ( std::abs ( first - next ) > DBL_EPSILON ) {
      return -1.0; // flag to indicate not the same
    }
  }

  return first;
}

double
CompositePlotter::
getOffset ( hippodraw::Axes::Type axis ) const
{
  int index = activePlotIndex ();

  if ( !( index < 0 ) ) {
    ProjectorBase * projector = getProjector ( index );
    return projector->getOffset ( axis );
  }

  vector< DataRep * >::const_iterator it = m_datareps.begin();

  string saxis;
  if ( axis == Axes::X ) {
    saxis = "X";
  } else if ( axis == Axes::Y ) {
    saxis = "Y";
  } else {
    saxis = "Z";
  }

  double first = -1.0;
  for ( ; it != m_datareps.end(); ++it ) {

    ProjectorBase * projector = (*it)->getProjector();

    if ( !( projector->isAxisBinned ( saxis ) ) ) {
      continue;
    }

    if ( first < 0.0 ) {
      first = projector->getOffset ( axis );
      continue;
    }

    double next = projector->getOffset ( axis );
    if ( first != next ) return -1.0; // flag indicating not same
  }

  return first;
}

void CompositePlotter::setRepColor ( const Color & color )
{
  if ( m_datarep_index < 0 ) return;

  DataRep * datarep = m_datareps[m_datarep_index];
  datarep->setRepColor ( color );
}

const Color & CompositePlotter::repColor ( ) const
{
  DataRep * datarep = 0;
  if ( m_datarep_index < 0 ) {
    datarep = m_datareps.front();
  }
  else {
    datarep = m_datareps[m_datarep_index];
  }

  return datarep->getRepColor ( );
}

void
CompositePlotter::
setAxisModel ( AxisModelBase * model, hippodraw::Axes::Type axis )
{

  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) m_x_axis = model;
  if ( axis == Axes::Y ) m_y_axis = model;
  if ( axis == Axes::Z && hasAxis ( Axes::Z ) ) m_z_axis = model;

  assert ( model );

  vector< DataRep * >:: iterator first = m_datareps.begin ();
  for ( ; first != m_datareps.end (); ++first ) {
    (*first)->setAxisModel ( axis,  model );
  }

  setAutoScaled ( false );
}

void CompositePlotter::setAutoRanging ( bool flag )
{
  setAutoRanging ( Axes::X, flag );
  setAutoRanging ( Axes::Y, flag );
  if ( hasAxis ( Axes::Z ) ) setAutoRanging ( Axes::Z, flag );
}

void CompositePlotter::setReverse ( bool flag )
{
  m_reverse = flag;
}

bool CompositePlotter::isReverse () const
{
  return m_reverse;
}

TransformBase *
CompositePlotter:: 
getTransform () const
{
  return m_transform;
}

/* virtual */
void CompositePlotter::setTransform ( TransformBase * tf )
{
  BinaryTransform * p = dynamic_cast< BinaryTransform * > ( tf );
  if (p) {
     TransformBase * tmp = tf -> clone ();
     std::swap ( tmp, m_transform );
     delete tmp;

     setAutoScaled ( false );
  } else {
     std::string what("CompositePlotter::setTransform: ");
     what += "Require a BinaryTransform in this context.";
     throw PlotterException(what);
  }
}

/** @todo Needs to be renamed.
 */
void
CompositePlotter::
toUserXY ( double x, double y, bool scaled, 
	   double & ux, double & uy ) const
{
  BinaryTransform * transform 
    = dynamic_cast < BinaryTransform * > ( m_transform );
  assert ( transform );

  transform -> inverseTransform ( x, y );

  ux = processReturnValue ( x, Axes::X, scaled ); 
  uy = processReturnValue ( y, Axes::Y, scaled ); 
}

NTuple *
CompositePlotter::
createPickTuple ( )
{
  unsigned int size = 3;
  vector < string > labels;
  labels.reserve ( 5 );

  labels.push_back ( "Item number" );
  labels.push_back ( getLabel ( Axes::X ) );
  labels.push_back ( getLabel ( Axes::Y ) );
 
 if ( hasAxis ( Axes::Z ) ) {
    size = 4;
    labels.push_back ( getLabel ( Axes::Z ) );
    if ( isAxisScaled ( Axes::Z ) ) {
      size = 5;
      labels.push_back ( "Density" );
    }
  }
  else { // no Z axis
    if ( isAxisScaled ( Axes::Y ) ) {
      size = 4;
      labels.push_back ( "Density" );
    }
  }

  NTuple * ntuple = new NTuple ( labels );

  return ntuple;
}

double
CompositePlotter::
getZValue ( double x, double y, bool scaled ) const
{
  DataRep * datarep = selectedDataRep ();
  if ( !datarep ){
    datarep = getDataRep ( 0 );
  }
  assert ( datarep );
  
  ProjectorBase * proj = datarep->getProjector();
  assert ( proj );

  double retval =  proj->getZValue ( x, y );
  
  return processReturnValue ( retval, Axes::Z, scaled );
}

void
CompositePlotter::
fillPickedPoint ( double x, double y, std::vector < double > & picked ) const
{
  BinaryTransform * tf
    = dynamic_cast < BinaryTransform * > ( m_transform );
  assert ( tf );

  bool yes = tf -> inverseTransform ( x, y );
  if ( !yes ) 
    {
      picked.clear ();
      return;
    }

  // Consider fits image with transform.
  if (m_fits_transform)
    {
      BinaryTransform * tf_fits
	= dynamic_cast < BinaryTransform * > ( m_fits_transform );
      bool yes = tf_fits -> inverseTransform ( x, y );
      if ( !yes )
	{
	  picked.clear();
	  return;
	}
    }

  double ux = processReturnValue ( x, Axes::X, true );
  double uy = processReturnValue ( y, Axes::Y, true );

  // Note the inverse transform ( as mentioned in the documentation )
  // does not take care of rotation which is why it is taken care over
  // here in explicit fashion.
  if ( tf->isPeriodic() )
    {
      const PeriodicBinaryTransform * tp 
	= dynamic_cast < const PeriodicBinaryTransform * > ( tf );
      
      double xoffset = tp->rotation(Axes::X);
      double yoffset = tp->rotation(Axes::Y);
      
      ux   = tp->moduloAddX( ux, xoffset/UNITS_RADIANS*tp->units(Axes::X) );
      uy   = tp->moduloAddY( uy, yoffset/UNITS_RADIANS*tp->units(Axes::Y) );
    }
  picked.clear ();
  picked.push_back ( 0.0 ); // item number
  picked.push_back ( ux ); 
  picked.push_back ( uy ); 
  if ( hasAxis ( Axes::Z ) ) {
    if ( isAxisScaled ( Axes::Z ) ) {
      picked.push_back ( getZValue ( ux, uy, true ) );  // scaled value
      picked.push_back ( getZValue ( ux, uy, false ) ); // density
      picked[0] = 3.0;
    }
    else { // not scaled
      picked.push_back ( getZValue ( ux, uy, false ) );
      picked[0] = 2.0;
    }
  }
  else { // no Z axis
    if ( isAxisScaled ( Axes::Y ) ) {
      picked.push_back ( processReturnValue ( y, Axes::Y, false ) ); // density
      picked[0] = 1.0;
    }
  }

}

double
CompositePlotter::
processReturnValue ( double retval, 
		     hippodraw::Axes::Type axis,
		     bool scaled ) const
{

  DataRep * datarep = mouseSelectedDataRep ();
  if ( !datarep ){
    datarep = getDataRep ( 0 );
  }
  assert ( datarep );
  
  ProjectorBase * proj = datarep->getProjector();
  assert ( proj );
  
  AxisModelBase * a = proj->getAxisModel ( axis );
  assert ( a );
  
  double sf = a->getScaleFactor();
  
  double scaledRetval = retval * sf;

  if ( scaled ) {
    retval = retval * sf;
  }
  
  const Range r = a->getRange ( true );
  
  // Fix the getZValue() bug. Need better fix.
  // Is rounding to range edge required for Axes::X and Axes::y ?
  if ( axis==Axes::Z && scaledRetval == 0.0 ) return 0.0;

  if ( scaledRetval > r.high() ) {
    if ( scaled ) {
      return r.high();
    }
    else {
      return r.high() / sf;
    }
  }
  
  if ( scaledRetval < r.low() ) {
    if ( scaled ) {
      return r.low();
    }
    else {
      return r.low() / sf;
    }
  }
  
  return retval;
}

DataRep * CompositePlotter::mouseSelectedDataRep() const
{
  return selectedDataRep();
}

void
CompositePlotter::
addValues ( const std::vector < double > & v )
{
  DataRep * rep = getDataRep ( 0 );
  rep->addValues ( v );
}

NTuple *
CompositePlotter::
createNTuple () const
{
  DataRep * rep = selectedDataRep ();
  if ( rep == 0 ) {
    rep = getDataRep ( 0 );
  }
  NTuple * ntuple = rep -> createNTuple ();

  return ntuple;
}

void
CompositePlotter::
update ()
{
  vector < DataRep * > :: iterator first = m_datareps.begin ();

  while ( first != m_datareps.end () ) {
    (*first++) -> update ();
  }
}

void
CompositePlotter::
setAutoTicks ( hippodraw::Axes::Type axis, bool yes )
{
  AxisModelBase * model = getAxisModel ( axis );

  model -> setAutoTicks ( yes );
}

void
CompositePlotter::
setTicks ( hippodraw::Axes::Type axis,
	   const std::vector < AxisTick > & ticks )
{
  AxisModelBase * model = getAxisModel ( axis );

  model -> setTicks ( ticks );
}

bool
CompositePlotter::
isTargetable ( ) const
{
  bool yes = false;
  std::size_t number = m_datareps.size ();

  if ( number == 1 ) {
    DataRep * datarep = m_datareps.front();
    yes = datarep -> isTargetable ();
  }
  else {
    int targets = 0;
    for ( std::size_t i = 0; i < number; i++ ) {
      DataRep * datarep = m_datareps[i];
      if ( datarep -> isTargetable () &&
	   datarep -> isSelected () ) {
	targets ++;
      }
    }
    if ( targets == 1 ) yes = true;
  }

  return yes;
}

DataRep *
CompositePlotter::
getTarget () const
{
  DataRep * rep = 0;
  std::size_t size = m_datareps.size ();

  for ( std::size_t i = 0; i < size; i++ ) {
    DataRep * dr = m_datareps[i];
    if ( dr -> isSelected () ) {
      rep = dr;
      break;
    }
  }

  return rep;
}

int
CompositePlotter::
indexOf ( const DataRep * rep ) const
{
  int index = -1;
  std::size_t size = m_datareps.size();
  for ( std::size_t i = 0; i < size; i++ ) {
    DataRep * dr = m_datareps[i];
    if ( dr == rep ) {
      index = i;
      break;
    }
  }

  return index;
}

bool
CompositePlotter::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) return m_x_axis != 0;
  if ( axis == Axes::Y ) return m_y_axis != 0;
  if ( axis == Axes::Z ) return m_z_axis != 0;

  return false;
}


AxisModelBase *
CompositePlotter::
getAxisModel ( hippodraw::Axes::Type axis ) const
{
  if ( axis == Axes::X ) return m_x_axis;
  else if ( axis == Axes::Y ) return m_y_axis;
  else if ( axis == Axes::Z ) return m_z_axis;

  assert ( false );

  return 0;
}

/* @request @@@ Could we change all asserts of this to type (I count
   about 80 of them throughout the code) to tests which throw a
   runtime error exception instead?
*/
void
CompositePlotter::
setAutoRanging ( hippodraw::Axes::Type axis, bool flag )
{

  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  AxisModelBase * model = 0;
  if ( axis == Axes::X ) model = m_x_axis;
  else if ( axis == Axes::Y ) model = m_y_axis;
  else 
    {
      if(!m_z_axis)return;
      model = m_z_axis;
    }
  
  assert (model);

  model->setAutoRanging ( flag );
  if ( flag == false ) return;

  m_x_axis->setEmpty();
  m_y_axis->setEmpty();
  if ( m_z_axis ) m_z_axis->setEmpty();

  setAutoScaled ( false );
}

bool
CompositePlotter::
isAutoRanging ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  if ( axis == Axes::X ) {
    return m_x_axis->isAutoRanging ( );
  } else if ( axis == Axes::Y ) {
    return m_y_axis->isAutoRanging ( );
  }
  // else Z
  if( !m_z_axis ) return false;

  return m_z_axis->isAutoRanging ( );
}

/** @bug Parameter adjust_width is not used
 */
void
CompositePlotter::
setRangePrivate ( hippodraw::Axes::Type axis, const Range & range, 
		  bool scaled, bool ) //adjust_width )
{
  BinaryTransform * transform
    = dynamic_cast< BinaryTransform * > ( m_transform );
  if ( axis == Axes::X ) {
    m_x_axis->setRange ( range, scaled );
    const Range & current = m_x_axis->getRange ( false );
    const Range & limit = transform->limitX ();
    m_x_axis->setIntersectRange ( current, limit );
    m_x_axis->setAutoRanging ( false );
  }
  else if ( axis == Axes::Y ) {
    m_y_axis->setRange ( range, scaled );
    const Range & current = m_y_axis->getRange ( false );
    const Range & limit = transform->limitY ();
    m_y_axis->setIntersectRange ( current, limit );
    m_y_axis->setAutoRanging ( false );
  }
  else if ( axis == Axes::Z ) {
    m_z_axis->setRange ( range, scaled );
    const Range & current = m_z_axis->getRange ( false );
    const Range & limit = transform->limitZ ();
    m_z_axis->setIntersectRange ( current, limit );
    m_z_axis->setAutoRanging ( false );
  }
}

void
CompositePlotter::
setLowRange ( hippodraw::Axes::Type  type,
	      int parm, bool dragging )
{
  AxisModelBase * model = 0;

  if ( type == Axes::X ) model = m_x_axis;
  else if ( type == Axes::Y ) model = m_y_axis;
  else if ( type == Axes::Z ) model = m_z_axis;
  assert ( model );

  Range new_range = model->calcLow ( parm, dragging );
  setRangePrivate ( type, new_range );

  setRange ( type, new_range, false, false ); // so DataRep see it.
}

void
CompositePlotter::
setHighRange ( hippodraw::Axes::Type type,
	       int parm, bool dragging )
{
  AxisModelBase * model = 0;

  if ( type == Axes::X ) model = m_x_axis;
  else if ( type == Axes::Y ) model = m_y_axis;
  else if ( type == Axes::Z ) model = m_z_axis;
  assert ( model );

  Range new_range = model->calcHigh ( parm, dragging );

  setRangePrivate ( type, new_range );
  setRange ( type, new_range, false, false ); // so DataRep see it.
}

const Range & 
CompositePlotter::
getRange ( hippodraw::Axes::Type axis, bool scaled ) const
{
  bool ok = axis == Axes::X || axis == Axes::Y || axis == Axes::Z;
  if ( ok == false ||
       ( axis == Axes::Z && m_z_axis == 0 ) ) {
    string what ( "PlotterBase::getRange: " );
    what += "This plotter does not have such axis";
    throw PlotterException ( what );
  }

  if ( axis == Axes::X ) return m_x_axis->getRange ( scaled );

  if ( axis == Axes::Y ) return m_y_axis->getRange ( scaled );
  
  return m_z_axis->getRange ( scaled );
}

void
CompositePlotter::
setScaleFactor ( hippodraw::Axes::Type axis, double factor )
{
  if ( axis == Axes::X ) {
    m_x_axis->setScaleFactor ( factor );
  }
  else if ( axis == Axes::Y ) {
    m_y_axis->setScaleFactor ( factor );
  }
  else if ( axis == Axes::Z ) {
    if ( m_z_axis ) m_z_axis->setScaleFactor ( factor );
  }
}

double
CompositePlotter::
getScaleFactor ( hippodraw::Axes::Type axis ) const
{
  double factor = 1.0;
  switch ( axis ) {
  case Axes::X :
    factor = m_x_axis -> getScaleFactor();
    break;
  case Axes::Y :
    factor = m_y_axis -> getScaleFactor ();
    break;
  case Axes::Z :
    factor = m_z_axis -> getScaleFactor ();
    break;
  default :
    break;
  }
  return factor;
}

bool
CompositePlotter::
isAxisScaled ( hippodraw::Axes::Type axis ) const
{
  bool yes = false;
  switch ( axis ) {
  case Axes::X :
    yes = m_x_axis -> isScaling();
    break;
  case Axes::Y :
    yes = m_y_axis -> isScaling();
    break;
  case Axes::Z :
    yes = m_z_axis -> isScaling();
    break;
  default :
    break;
  }

  return yes;
}


void
CompositePlotter::
setScaling ( hippodraw::Axes::Type axis, bool on )
{
  if ( axis == Axes::X ) {
    m_x_axis->setScaling ( on );
  }
  else if ( axis == Axes::Y ) {
    m_y_axis->setScaling ( on );
  }
  else if ( axis == Axes::Z ) {
    if ( m_z_axis ) m_z_axis->setScaling ( on );
  }
}

void
CompositePlotter::
setLabel ( hippodraw::Axes::Type axis, const std::string & value )
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) m_x_label = value;
  else if ( axis == Axes::Y ) m_y_label = value;
  else if ( axis == Axes::Z ) m_z_label = value;
}

const string &
CompositePlotter::
getLabel ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  ProjectorBase * projector = activeProjector();

  if ( axis == Axes::X ) {
    if ( projector == 0 || m_x_label != "%x" ) return m_x_label;
    else return projector->getXLabel ( );
  }
  if ( axis == Axes::Y ) {
    if ( projector == 0 || m_y_label != "%y" ) return m_y_label;
    else return projector->getYLabel ( );
  }
  // Z
  if ( projector == 0 || m_z_label != "%z" ) return m_z_label;
  return projector->getZLabel ( );
}

const string &
CompositePlotter::
getInternalLabel ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    return m_x_label;
  }
  else if ( axis == Axes::Y ) {
    return m_y_label;
  }
  // else Z
  return m_z_label;
}

double
CompositePlotter::
getAspectRatio () const
{
  if ( m_transform == 0 ) return 0.0;

  return m_transform->aspectRatio ();
}

void
CompositePlotter::
checkAxisScaling ( )
{
  int number = getNumDataReps ();
  if ( number < 2 ) return;

  bool wants_scaling = false;
  for ( int i = 0; i < number; i++ ) {
    ProjectorBase * projector = getProjector ( i );
    wants_scaling |= projector->wantsScaleFactor ( "Y" );
  }

  if ( wants_scaling == true ) {

    double width = getBinWidth ( Axes::X );

    if ( width <= 0.0 ) {
      setScaling ( Axes::Y, false );
    }
    else {
      setScaleFactor ( Axes::Y, width );
    }
  }

}

void
CompositePlotter::
setAutoScaled ( bool flag )
{
  m_has_autoscaled = flag;
}

bool
CompositePlotter::
hasAutoScaled () const
{
  return m_has_autoscaled;
}

bool
CompositePlotter::
checkAutoScale ()
{
  bool has_scaled = hasAutoScaled ();
  bool is_dirty = isDirty ();

  bool yes = ( has_scaled == false ) || is_dirty == true;

  return yes;
}

void
CompositePlotter::
prepareToDraw ()
{
  Range x_range = m_x_axis->getRange ( false );
  Range y_range = m_y_axis->getRange ( false );
  
  // Validate the copy of axis range and use it to set the ranges on the axis 
  TransformBase * tbase = getTransform ();
  BinaryTransform * transform = dynamic_cast < BinaryTransform * > ( tbase );
  
  transform->validate ( x_range, y_range );

  double z_lo = 0.;
  double z_hi = 0.;

  if ( m_x_axis -> isAutoTicks () ) {
    const vector < AxisTick > & x_ticks 
      = transform -> setTicks ( *m_x_axis, Axes::X );
    m_x_axis -> setTicks ( x_ticks );
  }

  if ( m_y_axis -> isAutoTicks () ) {
    const vector < AxisTick > & y_ticks 
      = transform -> setTicks ( *m_y_axis, Axes::Y );
    m_y_axis -> setTicks ( y_ticks );
  }

  if ( m_has_z ) {
    const Range & z_range = m_z_axis->getRange ( false );
    if ( m_z_axis -> isAutoTicks () ) {
      const vector < AxisTick > & z_ticks 
	= transform -> setTicks ( *m_z_axis, Axes::Z );
      m_z_axis -> setTicks ( z_ticks );
    }
    z_lo = z_range.low ();
    z_hi = z_range.high ();

    transform->transformZ ( z_lo );
    transform->transformZ ( z_hi );
  }

  m_x_axis->setRange ( x_range, false );
  m_y_axis->setRange ( y_range, false );
}

Rect
CompositePlotter::
calcUserRectangle ( ) const
{
  BinaryTransform * transform 
    = dynamic_cast < BinaryTransform * > ( m_transform );
  const Range & x_range = m_x_axis -> getRange ( false );
  const Range & y_range = m_y_axis -> getRange ( false );

  Rect rect = transform -> calcRectangle ( x_range, y_range );
  if ( m_has_z ) {
    const Range & range = m_z_axis -> getRange ( false );
    double z_lo = range.low ();
    double z_hi = range.high ();
    transform -> transformZ ( z_lo );
    transform -> transformZ ( z_hi );
    rect.setZ ( z_lo );
    rect.setDepth ( z_hi - z_lo ); 
  }

  return rect;
}

Rect
CompositePlotter::
calcRawRectangle() const
{
  const Range & x_range = m_x_axis -> getRange ( false );
  const Range & y_range = m_y_axis -> getRange ( false );

  double x_lo = x_range.low();
  double x_hi = x_range.high();

  double y_lo = y_range.low();
  double y_hi = y_range.high();

  Rect rect = Rect (x_lo, y_lo, x_hi-x_lo, y_hi-y_lo);

  return rect;
}


void
CompositePlotter::
drawAxisRep ( AxisRepBase * rep, ViewBase * view,
	      bool do_y, bool do_z )
{
  rep->setFontSize ( m_x_axis, m_y_axis, m_z_axis, *view );

  rep->drawXLabels( *m_x_axis, *view, ( m_x_label == "%x" ) ?
  			   getLabel ( Axes::X ) : m_x_label );

  if ( do_y ) {
    rep->drawYLabels( *m_y_axis, *view, ( m_y_label == "%y" ) ?
		      getLabel ( Axes::Y ) : m_y_label );
  }

  if ( m_has_z && do_z ) {
    rep->drawZLabels( *m_z_axis, *view, ( m_z_label == "%z" ) ?
			     getLabel ( Axes::Z ) : m_z_label );
    const BinToColor * btc = getValueRep ();
    if ( btc != 0 ) { // if one exists
      rep ->drawColorScale ( *btc, *view );
    }
  }

  const Range & x_range = m_x_axis -> getRange ( false );
  const Range & y_range = m_y_axis -> getRange ( false );

  rep->drawAxesLines( *m_transform, *view, x_range, y_range );

  /* Draw X ticks if needed by the transform */
  BinaryTransform * transform 
    = dynamic_cast < BinaryTransform * > ( m_transform );
  if ( transform->needsXTicks() ) {
    rep->drawAllXTicks( *m_x_axis, *m_y_axis, *transform, *view );
  }

  /* Draw X ticks if needed by the transform */
  if ( do_y && transform->needsYTicks() ) {
    rep->drawAllYTicks( *m_x_axis, *m_y_axis, *transform, *view );
  }

  if ( do_z && m_has_z ) {
    rep->drawAllZTicks( *m_z_axis, *transform, *view );
  }
  /* If needed draw the axis lines */
  if ( m_show_grid ) {
    rep->drawGridLines( *m_x_axis, *m_y_axis, *transform, *view );
  }
}

void
CompositePlotter::
setEnableZ ( bool yes )
{
  if ( yes ) {
    m_z_axis = new AxisModelLinear ( PLOTTOP, PLOTTOP );
  }
  else { 
    if ( m_z_axis != 0 ) delete m_z_axis;
  }

  m_has_z = yes;
}

void
CompositePlotter::
fillCutList ( std::vector < const TupleCut * > & cuts ) const
{
  DataRepList_t::const_iterator first = m_datareps.begin();
  while ( first != m_datareps.end () ) {
    const DataRep * rep = *first++;
    bool yes = rep -> hasCut ();
    if ( yes ) {
      const vector < TupleCut > & tcuts = rep ->getCuts ();
      unsigned int size = tcuts.size ();
      for ( unsigned int i = 0; i < size; i++ ) {
	cuts.push_back ( &tcuts[i] );
      }
    }
  }
}

void
CompositePlotter::
setCutRangeAt ( const Range & range, unsigned int i )
{
  DataRep * rep = selectedDataRep ();
  rep -> setCutRangeAt ( range, i );
}

void
CompositePlotter::
setShowGrid ( bool flag )
{
  m_show_grid = flag;
}

bool
CompositePlotter::
getShowGrid ()
{
  return m_show_grid;
}

void
CompositePlotter::
setBoxEdge(bool flag)
{
  m_box_edge = flag;
}

bool
CompositePlotter::
getBoxEdge()
{
  return m_box_edge;
}


void
CompositePlotter::
setFitsTransform (const std::string & transform)
{
  TransformFactory * factory = TransformFactory::instance();
  m_fits_transform = factory->createTransform(transform);
}

TransformBase *
CompositePlotter:: 
getFitsTransform () const
{
  return m_fits_transform;
}

void
CompositePlotter::
setMinEntries(int entries)
{
  vector < DataRep * >:: iterator it = m_datareps.begin();
  while ( it != m_datareps.end() ) {
    ProjectorBase * projector = (*it++)->getProjector ();
    projector->setMinEntries ( entries );
  }
}

int
CompositePlotter::
getMinEntries ()
{
  DataRep * datarep = selectedDataRep ();
  if ( !datarep ){
    datarep = getDataRep ( 0 );
  }
  assert ( datarep );
  
  ProjectorBase * proj = datarep->getProjector();
  assert ( proj );

  return proj->getMinEntries ();
}

void
CompositePlotter::
toggleBoxEdge(DataRep* datarep)
{
  RepBase* rb = datarep->getRepresentation();
  ColorBoxPointRep * cb = dynamic_cast< ColorBoxPointRep * > ( rb );
  if (cb) cb->setBoxEdge(m_box_edge);
}

bool
CompositePlotter::
isImageConvertable () const
{
  bool yes = false;
  if ( m_datareps.size() == 1 ) {
    yes = m_datareps.front () -> isImageConvertable ();
  }

  return yes;
}
