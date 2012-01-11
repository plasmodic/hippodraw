/** @file

hippodraw::XyPlotter class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: XyPlotter.cxx,v 1.195 2007/09/06 19:45:42 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "XyPlotter.h"

#include "CompositePlotter.h"

#include "axes/AxisModelBase.h"
#include "datareps/DataRep.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "graphics/ViewBase.h"

#include "projectors/ProjectorBase.h"
#include "transforms/PeriodicBinaryTransform.h"
#include "reps/AxisRep2D.h"
#include "reps/AxisRepColor.h"
#include "reps/RepBase.h"

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::list;
using std::string;
using std::vector;
#endif

using namespace hippodraw;

XyPlotter::XyPlotter (  const std::string & name  )
  : PlotterBase ( name )
{
  m_plotter = new CompositePlotter ();
  m_plotters.push_back ( m_plotter );
  m_axis_rep = new AxisRep2D ();
  view_change_allowed = true;

  m_margin_top = 0.0;
  m_margin_bottom = 0.0;
  m_margin_left = 0.0;
  m_margin_z = 0.0;

  m_need_update = false;
}

XyPlotter::XyPlotter ( const XyPlotter & plotter )
  : PlotterBase ( plotter )
{
  m_plotter = plotter.m_plotter -> clone ();
  m_plotters.push_back ( m_plotter );
  m_axis_rep = new AxisRep2D ();
  m_plotter -> setAllAxisModels ();
  view_change_allowed = true;

  m_margin_top = plotter.m_margin_top;
  m_margin_bottom = plotter.m_margin_bottom;
  m_margin_left = plotter.m_margin_left;
  m_margin_z = plotter.m_margin_z;

  m_need_update = plotter.m_need_update;
}

XyPlotter::~XyPlotter()
{
  PlotterList_t ::iterator first = m_plotters.begin();
  AxisModelBase * model = (*first) -> getAxisModel ( Axes::X );
  while ( first != m_plotters.end () ) {
    delete *first++;
  }
  delete model;
  delete m_axis_rep;
}

PlotterBase * XyPlotter::clone()
{
  return new XyPlotter( *this );
}

void
XyPlotter::
checkAutoScale ()
{
  PlotterList_t::size_type size = m_plotters.size ();

  bool yes = false;
  for ( PlotterList_t::size_type i = 0; i < size; i++ ) {
    CompositePlotter * plotter = m_plotters [ i ];
    bool y = plotter -> checkAutoScale ();
    yes |= y;
  }

  if ( yes ) {
    CompositePlotter * plotter = m_plotters.front ();
    AxisModelBase * model = plotter -> getAxisModel ( Axes::X );
    model -> setEmpty ();
    for ( PlotterList_t::size_type i = 0; i < size; i++ ) {
      plotter = m_plotters [ i ];
      plotter -> autoScale ( model, Axes::X );
      plotter -> autoScale ( Axes::Y );
      if ( plotter -> hasAxis ( Axes::Z ) &&
	   plotter -> isAutoRanging ( Axes::Z ) ) {
	plotter -> autoScale ( Axes::Z );
      }
      plotter -> setAutoScaled (  true );
    }
  }
}

void
XyPlotter::
prepareToDraw ( )
{
  checkAutoScale ();

  PlotterList_t::size_type size = m_plotters.size ();

  for ( PlotterList_t::size_type i = 0; i < size; i++ ) {
    m_plotter = m_plotters [ i ];
    m_plotter -> prepareToDraw ();
  }
}

void XyPlotter::drawIn ( ViewBase * view )
{
  // m_need_update is set by inspector. update the draw rect of the view. 
  if (m_need_update) {
    view->updateDrawRect();
    m_need_update = false;
  }

  // Draw values before drawing axis.
  drawProjValues ( view );

  m_axis_rep->beginPlot ( *view );
  bool do_y = m_plotters.size() == 1;
  if ( m_active_index >= 0 ) do_y |= true;
  m_user_rect = m_plotter -> calcUserRectangle ();
  m_raw_rect = m_plotter -> calcRawRectangle();

  if ( getNumDataReps () != 0 ) {
    /* Draws last because it uses a larger font. */
    m_axis_rep->drawTitle( *view, ( m_title == "%t" ) ?
			   getTitle () : m_title );
    
  }
  m_plotter -> drawAxisRep ( m_axis_rep, view, do_y, true );


}

void
XyPlotter::
drawCrossHairs ( ViewBase * view )
{
  TransformBase * transform = getTransform ();

  m_axis_rep->drawCrossHairs ( m_crossX, m_crossY, *transform, *view );
}

NTuple *
XyPlotter::
createNTuple () const
{
  NTuple * ntuple = m_plotter -> createNTuple ();

  const string & label_x = getLabel ( Axes::X );
  ntuple -> setLabelAt ( label_x, 0 );
  if ( m_plotter -> hasAxis ( Axes::Z ) ) {
    const string & label_y = getLabel ( Axes::Y );
    ntuple -> setLabelAt ( label_y, 1 );
    const string & label_z = getLabel ( Axes::Z );
    ntuple -> setLabelAt ( label_z, 2 );
  }
  else {
    ProjectorBase * projector = activeProjector ();
    const string & label_y = projector -> getYLabel ( true );
    ntuple -> setLabelAt ( label_y, 1 );
  }
  return ntuple;
}

bool
XyPlotter::
wantsPixmap ( ) const
{
  return m_plotter -> hasAxis ( Axes::Z );
}

void
XyPlotter::
setEnableZ ( bool yes )
{
  m_plotter -> setEnableZ ( yes );

  assert ( m_axis_rep );
  delete m_axis_rep;
  m_axis_rep = new AxisRepColor ();
}

void
XyPlotter::
setLabelFont ( FontBase* font, hippodraw::Axes::Type axes )
{
  if( axes == hippodraw::Axes::X )
    m_axis_rep -> setXLabelFont( font );
  else if( axes == hippodraw::Axes::Y )
    m_axis_rep -> setYLabelFont( font );
  else if( axes == hippodraw::Axes::Z )
    m_axis_rep -> setZLabelFont( font );
}

FontBase*
XyPlotter::
labelFont ( hippodraw::Axes::Type axes ) const
{
  if( axes == hippodraw::Axes::X )
    return m_axis_rep -> xLabelFont();
  else if( axes == hippodraw::Axes::Y )
    return m_axis_rep -> yLabelFont();
  else if( axes == hippodraw::Axes::Z )
    return m_axis_rep -> zLabelFont();

  return 0;
}


void
XyPlotter::
setTitleFont( FontBase* font )
{
  m_axis_rep -> setTitleFont( font );
}
	

const FontBase *
XyPlotter::
titleFont( ) const
{
  return m_axis_rep -> titleFont();
}

void
XyPlotter::
setActivePlot ( int index, bool redraw )
{
  m_active_index = index;
  bool done = false;
  PlotterList_t::iterator first = m_plotters.begin();
  while ( first != m_plotters.end () ) {
    CompositePlotter * plotter = *first++;
    if ( index < 0 ) {
      plotter -> setActivePlot ( -1, redraw );
      continue;
    }
    else {
      int num = plotter -> getNumDataReps ();
      if ( index < num && 
	   done == false ) {
	plotter -> setActivePlot ( index, redraw );
	m_plotter = plotter;
	done = true;
      }
      else {
	plotter -> setActivePlot ( -2, redraw );
	index -= num;
      }
    }
  }

}

int
XyPlotter::
activePlotIndex () const
{
  return m_active_index;
}

void
XyPlotter::
addDataRep ( DataRep * rep )
{
  m_plotter -> addDataRep ( rep );
  rep -> addObserver ( this );

  unsigned int number = m_plotter -> getNumDataReps ();
  if ( number == 1 ) {
    m_active_index = 0;
  }
  else {
    m_active_index = -1;
  }
  autoScale ();

  notifyObservers ();
}

void
XyPlotter::
addDataRepStacked ( DataRep * rep )
{
  m_plotter = new CompositePlotter ();
  m_plotters.push_back ( m_plotter );
  addDataRep ( rep );

  CompositePlotter * plotter = m_plotters.front ();
  AxisModelBase * model = plotter -> getAxisModel ( Axes::X );
  m_plotter -> setAxisModel ( model, Axes::X );

  m_active_index = -1;

  setAutoRanging ( true );
  autoScale ();

  notifyObservers ();
}

void
XyPlotter::
removeDataRep ( DataRep * rep )
{
  PlotterList_t::iterator ip = m_plotters.begin();
  while ( ip != m_plotters.end() ) {
    CompositePlotter * plotter = *ip;
    plotter -> removeDataRep ( rep ); // does nothing if not there
    int number = plotter -> getNumDataReps ();
    if ( number == 0 ) {
      delete *ip;
      m_plotters.erase ( ip ); 
      break;
    }
    ip++;
  }
  notifyObservers ();
}

void
XyPlotter::
autoScale ()
{
  m_plotter -> autoScale ();
  notifyObservers ();
}

void
XyPlotter::
setValueRep ( BinToColor * rep )
{
  m_plotter -> setValueRep ( rep );
  notifyObservers ();
}

void
XyPlotter::
setAutoRanging ( bool flag )
{
  m_plotter -> setAutoRanging ( flag );
  if ( flag == true ) notifyObservers ();
}

void
XyPlotter::
setReverse ( bool flag )
{
  m_plotter -> setReverse ( flag );
  notifyObservers ();
}


void
XyPlotter::
setAutoRanging ( hippodraw::Axes::Type axis, bool flag )
{
  m_plotter -> setAutoRanging ( axis, flag );
  if ( flag == true ) notifyObservers ();
}

void
XyPlotter::
setTransform ( TransformBase * transform )
{
  for (  PlotterList_t::size_type i = 0; i < m_plotters.size(); i ++ ) {
    CompositePlotter * plotter = m_plotters[i];
    plotter -> setTransform ( transform );
  }

  notifyObservers ();
}

NTuple *
XyPlotter::
createPickTuple ()
{
  NTuple * ntuple =  m_plotter -> createPickTuple ();

  string name ( "Pick table for " );
  const string & title = getTitle ();
  name += title;
  ntuple -> setName ( name );
  ntuple -> setTitle ( name );

  return ntuple;
}

void
XyPlotter::
fillPickedPointFrom ( double mx, double my, 
		      std::vector < double > & picked ) const
{
  m_plotter -> fillPickedPoint ( mx, my, picked );
}

void
XyPlotter::
setLowRange ( hippodraw::Axes::Type  type,
	      int parm, bool dragging )
{
  m_plotter -> setLowRange ( type, parm, dragging );
  notifyObservers ();
}

void
XyPlotter::
setHighRange ( hippodraw::Axes::Type type,
	       int parm, bool dragging )
{
  m_plotter -> setHighRange ( type, parm, dragging );
  notifyObservers ();
}

void
XyPlotter::
setScaling ( hippodraw::Axes::Type axis, bool on )
{
  m_plotter -> setScaling ( axis, on );
  notifyObservers ();
}

double
XyPlotter::
getAspectRatio () const
{
  double ratio = m_aspect_ratio;
  PlotterList_t::size_type size = m_plotters.size ();
  if ( ratio == 0.0 ) {
    for ( PlotterList_t::size_type i = 0; i < size; i++ ) {
      CompositePlotter * plotter = m_plotters [ i ];
      double r = plotter -> getAspectRatio ();

      if ( r != 0.0 ) {
	ratio = r;
	break;
      }
    }
  }

  return ratio;
}

void
XyPlotter::
update ()
{
  m_plotter -> update ();
}

void
XyPlotter::
setRepresentation ( RepBase * rep )
{
  m_plotter -> setRepresentation ( rep );
}

RepBase *
XyPlotter::
representation () const
{
  return m_plotter -> representation ();
}

AxisModelBase *
XyPlotter::
getAxisModel ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getAxisModel ( axis );
}

void
XyPlotter::
setAxisModel ( AxisModelBase * model, hippodraw::Axes::Type axis )
{
  m_plotter -> setAxisModel ( model, axis );
}

void
XyPlotter::
setErrorDisplay ( hippodraw::Axes::Type axis, bool yes )
{
  m_plotter -> setErrorDisplay ( axis, yes );
}

bool
XyPlotter::
errorDisplay ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> errorDisplay ( axis );
}

void
XyPlotter::
setRepColor ( const Color & color )
{
  m_plotter -> setRepColor ( color );
}

const Color &
XyPlotter::
repColor () const
{
  int index = activePlotIndex ();
  if ( index < 0 ) index = 0;
  DataRep * rep = getDataRep ( index );

  return rep -> getRepColor ();
}

void
XyPlotter::
drawProjValues ( ViewBase * view )
{
  PlotterList_t::iterator first = m_plotters.begin();

  while ( first != m_plotters.end () ) {
    m_plotter = *first++;
    m_user_rect = m_plotter -> calcUserRectangle ( );
    m_raw_rect = m_plotter -> calcRawRectangle();
    m_plotter -> drawProjValues ( view );
  }
}

bool
XyPlotter::
hasAutoScaled () const
{
  return m_plotter -> hasAutoScaled ();
}

bool
XyPlotter::
hasZoomY () const
{
  bool retVal = true;
  
  for ( unsigned int i = 0; i < m_plotters.size () ; i++ )
    {
      retVal = retVal &= ( m_plotters[i] -> hasZoomY () ); 
    }
  return retVal != 0;
}

bool
XyPlotter::
hasNTupleBindings () const
{
  return m_plotter -> hasNTupleBindings ();
}

void
XyPlotter::
checkAxisScaling ( )
{
  m_plotter -> checkAxisScaling ();
}

ProjectorBase *
XyPlotter::
getProjector ( int i ) const
{
  const DataRep * rep = getDataRep ( i );

  return rep -> getProjector ();
}

ProjectorBase *
XyPlotter::
activeProjector () const
{
  return m_plotter -> activeProjector ();
}

DataRep *
XyPlotter::
getDataRep ( int index ) const
{
  DataRep * rep = 0;
  PlotterList_t::const_iterator first = m_plotters.begin ();
  while ( first != m_plotters.end () ) {
    CompositePlotter * plotter = *first++;
    int num = plotter -> getNumDataReps ();
    if ( index < num ) {
      rep = plotter -> getDataRep ( index );
      break;
    }
    index -= num;
  }

  return rep;
}

DataRep *
XyPlotter::
getTarget () const
{
  DataRep * rep = 0;

  if ( m_active_index >= 0 ) {
    rep = getDataRep ( m_active_index );
  }
  else {
    PlotterList_t::const_iterator first = m_plotters.begin();
    while ( first != m_plotters.end() ) {
      const CompositePlotter * plotter = *first++;
      int number = plotter -> getNumDataReps ();
      for ( int i = 0; i < number; i++ ) {
	DataRep * r = plotter -> getDataRep ( i );
	if ( r -> isTargetable () ) {
	  rep = r;
	  break;
	}
      }
      if ( rep != 0 ) break;
    }
  }

  return rep;
}

int
XyPlotter::
getNumDataReps () const
{
  int number = 0;
  PlotterList_t::const_iterator first = m_plotters.begin();

  while ( first != m_plotters.end() ) {
    CompositePlotter * plotter = *first++;
    number += plotter -> getNumDataReps ();
  }

  return number;
}

bool
XyPlotter::
isTargetable () const
{
  bool yes = false;

  if ( m_active_index >= 0 ) {
    const DataRep * rep = getDataRep ( m_active_index );
    assert ( rep != 0 );
    yes = rep -> isTargetable ();
  }
  else {
    int count = 0;
    PlotterList_t::const_iterator first = m_plotters.begin();
    while ( first != m_plotters.end() ) {
      const CompositePlotter * plotter = *first++;
      int number = plotter -> getNumDataReps ();
      for ( int i = 0; i < number; i++ ) {
	const DataRep * rep = plotter -> getDataRep ( i );
	if ( rep != 0 && rep -> isTargetable () ) count++;
      }
    }
    yes = count == 1;
  }

  return yes;
}

int
XyPlotter::
indexOf ( const DataRep * rep ) const
{
  int index = 0;
  PlotterList_t::const_iterator first = m_plotters.begin ();
  while ( first != m_plotters.end () ) {
    const CompositePlotter * plotter = *first++;
    int num = plotter -> getNumDataReps ();
    int i = plotter -> indexOf ( rep );
    if ( i >= 0 ) {
      index += i;
      break;
    }
    index += num;
  }

  return index;
}

double
XyPlotter::
getBinWidth ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getBinWidth ( axis );
}

void
XyPlotter::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  int index = activePlotIndex ();

  if ( index < 0 ) {
    PlotterList_t::iterator first = m_plotters.begin ();
    while ( first != m_plotters.end () ) {
      CompositePlotter * plotter = *first++;
      plotter -> setBinWidth ( axis, width );
    }
  }
  else {
    Range cur_range = getRange (axis, false );
    DataRep * rep = getDataRep ( index );
    const Range & range = rep -> setBinWidth ( axis, width );
    cur_range.setUnion ( range );
    setRange ( axis, cur_range, false, false );
  }
}

void
XyPlotter::
reset ( )
{
  m_plotter -> reset ( );
}

const Range &
XyPlotter::
getRange ( hippodraw::Axes::Type axis, bool scaled ) const
{
  return m_plotter -> getRange ( axis, scaled );
}

void
XyPlotter::
setRange ( hippodraw::Axes::Type axis, const Range & range,
	   bool scaled, bool adjust_width )
{
  m_plotter -> setRange ( axis, range, scaled, adjust_width );
}

double
XyPlotter::
getOffset ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getOffset ( axis );
}

TransformBase *
XyPlotter::
getTransform () const
{
  return m_plotter -> getTransform ();
}

void
XyPlotter::
addValues ( const std::vector < double > & v )
{
  m_plotter -> addValues ( v );
}

const string &
XyPlotter::
getInternalLabel ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getInternalLabel ( axis );
}

const string &
XyPlotter::
getLabel ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getLabel ( axis );
}

void
XyPlotter::
setLabel ( hippodraw::Axes::Type axis, const std::string & value )
{
  m_plotter -> setLabel ( axis, value );
  notifyObservers ();
}

int
XyPlotter::
getNumberOfEntries () const
{
  return m_plotter -> getNumberOfEntries ();
}

double
XyPlotter::
getPosRange ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getPosRange ( axis );
}

bool
XyPlotter::
hasAxis ( hippodraw::Axes::Type axis ) const
{
  bool yes = false;
  PlotterList_t::const_iterator first = m_plotters.begin ();

  while ( first != m_plotters.end () ) {
    CompositePlotter * plotter = *first++;
    yes |= plotter -> hasAxis ( axis );
  }

  return yes;
}

const BinToColor *
XyPlotter::
getValueRep () const
{
  return m_plotter -> getValueRep ();
}

bool
XyPlotter::
isAutoRanging ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> isAutoRanging ( axis );
}

bool
XyPlotter::
isReverse ( ) const
{
  return m_plotter -> isReverse ( );
}


void
XyPlotter::
matrixTranspose ( bool yes )
{
  m_plotter -> matrixTranspose ( yes );
}

DataRep *
XyPlotter::
selectedDataRep () const
{
  return m_plotter -> selectedDataRep ();
}

void
XyPlotter::
setAutoTicks ( hippodraw::Axes::Type axis, bool yes )
{
  m_plotter -> setAutoTicks ( axis, yes );
}

void
XyPlotter::
setTicks ( hippodraw::Axes::Type axis, const std::vector < AxisTick > & ticks )
{
  m_plotter -> setTicks ( axis, ticks );
}

void
XyPlotter::
setNumberOfBins ( hippodraw::Axes::Type axis, unsigned int number )
{
  m_plotter -> setNumberOfBins ( axis, number );
}

void
XyPlotter::
setOffset ( hippodraw::Axes::Type axis, double offset )
{
  m_plotter -> setOffset ( axis, offset );
}

void
XyPlotter::
fillCutList ( std::vector < const TupleCut * > & cuts ) const
{
  PlotterList_t::const_iterator first = m_plotters.begin();
  while ( first != m_plotters.end () ) {
    const CompositePlotter * plotter = *first++;
    plotter -> fillCutList ( cuts );
  }
}

TupleCut *
XyPlotter::
getCutAt ( unsigned int i )
{
  vector < const TupleCut * > cuts;
  fillCutList ( cuts );
  TupleCut * cut = const_cast < TupleCut * > ( cuts[i] );

  return cut;
}

void
XyPlotter::
setCutRangeAt ( const Range & range, unsigned int i )
{
  TupleCut * cut = getCutAt ( i );
  cut -> setRange ( range );

  DataRep * rep = getDataRep ( 0 );
  rep -> setDirty ();

  notifyObservers ();
}

Range
XyPlotter::
getCutRange ()
{
  TupleCut * cut = getCutAt ( 0 );
  Range range = cut -> getRange ();
  
  return range;
}




void
XyPlotter::
setCutInverted ( unsigned int i, bool yes )
{
  TupleCut * cut = getCutAt ( i );
  cut -> setInversion ( yes );

  DataRep * rep = getDataRep ( 0 );
  rep -> setDirty ();

  notifyObservers ();
}

void
XyPlotter::
setCutEnabled ( unsigned int i, bool yes )
{
  TupleCut * cut = getCutAt ( i );
  cut -> setEnabled ( yes );
  DataRep * rep = getDataRep ( 0 );
  rep -> setDirty ();

  notifyObservers ();
}


const std::vector <double> &
XyPlotter::
getZValues()
{
  TransformBase * transform = getTransform ();
  ProjectorBase * proj = activeProjector ();
  
  const BinaryTransform * bt 
    = dynamic_cast <const BinaryTransform *> ( transform );

  if ( bt->isPeriodic() )
    {
      return ( proj -> getZAfterTransform (transform) );
    }
  else
    {
      return ( proj -> getZValues() );
    }
  
}



const std::vector < unsigned int > &
XyPlotter::
getShape()
{
  TransformBase * transform = getTransform ();
  ProjectorBase * proj = activeProjector ();
  
  const BinaryTransform * bt 
    = dynamic_cast < const BinaryTransform * > ( transform );
  
  // Hammer or lambert
  if ( bt -> isPeriodic() )
    {
	m_periodic_shape.clear();
	m_periodic_shape.reserve(3);
	m_periodic_shape.push_back(bt->limitX().high() - bt->limitX().low());
	m_periodic_shape.push_back(bt->limitY().high() - bt->limitY().low());
	m_periodic_shape.push_back(2);

	return m_periodic_shape;
    }
  else
    {
      return ( proj -> getShape() );
    }
}

bool XyPlotter::getCutInversion ()
{
  TupleCut * cut = getCutAt ( 0 );
  return cut-> getInversion ();
}

void XyPlotter::setBoxEdge(bool flag)
{
  m_plotter -> setBoxEdge(flag);
  notifyObservers ();
}

bool XyPlotter::getBoxEdge ()
{
  return m_plotter->getBoxEdge();
}


void XyPlotter::setShowGrid( bool flag )
{
  m_plotter -> setShowGrid ( flag );
  notifyObservers ();
}

bool XyPlotter::getShowGrid()
{
  return m_plotter -> getShowGrid ();
}


void
XyPlotter::
setFitsTransform ( const std::string & transform )
{
  m_plotter -> setFitsTransform ( transform );
}

TransformBase *
XyPlotter::
getFitsTransform () const
{
  return m_plotter -> getFitsTransform ();
}

void
XyPlotter::
setMinEntries ( int entries )
{
  m_plotter -> setMinEntries(entries);
}

int
XyPlotter::
getMinEntries ()
{
  return m_plotter -> getMinEntries ();
}

void
XyPlotter::
setNeedUpdate( bool isChanged )
{
  m_need_update = isChanged;
}

bool
XyPlotter::
isImageConvertable () const
{
  bool yes = false;
  if ( m_plotters.size() == 1 ) {
    yes = m_plotters.front() -> isImageConvertable ();
  }

  return yes;
}

void
XyPlotter::
setTopMargin( double top ) 
{
  m_margin_top=top;
}

void
XyPlotter::
setZMargin( double z )
{
  m_margin_z = z;
}

void
XyPlotter::
setBottomMargin(double bottom)
{
  m_margin_bottom=bottom;
}

void
XyPlotter::
setLeftMargin(double left)
{
  m_margin_left=left;
}

double
XyPlotter::
getTopMargin()
{
  return m_margin_top;
}

double
XyPlotter::
getZMargin()
{
  return m_margin_z;
}


double
XyPlotter::
getBottomMargin()
{
  return m_margin_bottom;
}

double
XyPlotter::
getLeftMargin()
{
  return m_margin_left;
}

void
XyPlotter::
setScaleFactor ( hippodraw::Axes::Type axis, double factor )
{
  m_plotter -> setScaleFactor ( axis, factor );
}

double
XyPlotter::
getScaleFactor ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> getScaleFactor ( axis );
}

bool
XyPlotter::
isAxisScaled ( hippodraw::Axes::Type axis ) const
{
  return m_plotter -> isAxisScaled ( axis );
}
