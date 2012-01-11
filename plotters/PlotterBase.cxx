/** @file

hippodraw::PlotterBase class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PlotterBase.cxx,v 1.361 2007/09/06 19:45:41 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "PlotterBase.h"
#include "PlotterException.h"

#include "axes/AxisTick.h"
#include "datareps/DataRep.h"
#include "projectors/ProjectorBase.h"
#include "pattern/string_convert.h"

using namespace hippodraw;

using std::string;
using std::vector;

int PlotterBase::s_plotters(0);

PlotterBase::
PlotterBase ( ) 
   : m_aspect_ratio ( 0.0 ),
     m_name ( "" ),
     m_title( "%t" ),
     m_crossX ( 0 ),
     m_crossY ( 0 ),
     m_parent_plotter( 0 ),
     m_parent_datarep_index( -1 )

{
  current_range_saved = false;
   setPlotterId();
}

PlotterBase::
PlotterBase ( const std::string & name )
  : m_aspect_ratio ( 0.0 ),
    m_name ( name ),
    m_title( "%t" ),
    m_crossX ( 0 ),
    m_crossY ( 0 ),
    m_parent_plotter( 0 ),
    m_parent_datarep_index( -1 )
    
{
  current_range_saved = false;
   setPlotterId();
}

PlotterBase::PlotterBase ( const PlotterBase & plotter )
  : Observer (),
    Observable (),
    m_aspect_ratio ( plotter.m_aspect_ratio ),
    m_name ( plotter.m_name ),
    m_title( plotter.m_title ),
    m_parent_plotter( plotter.m_parent_plotter ),
    m_parent_datarep_index( plotter.m_parent_datarep_index ),
    m_views( plotter.m_views ),
    m_currentView( plotter.m_currentView )

{
  current_range_saved = false;
   setPlotterId();
}

PlotterBase::~PlotterBase()
{
  Observable::notifyObservers ( &hippodraw::Observer::willDelete );
}

const string & PlotterBase::name () const
{
  return m_name;
}

/* virtual */
void PlotterBase::update ( const Observable * )
{
  notifyObservers ();
}

void
PlotterBase::
prepareToDraw ()
{
  // does nothing
}

bool
PlotterBase::
hasAxis ( hippodraw::Axes::Type ) const
{
  return false;
}

bool
PlotterBase::
hasZoomY ( ) const
{
  return true;
}


AxisModelBase *
PlotterBase::
getAxisModel ( hippodraw::Axes::Type ) const
{
  return 0;
}

void PlotterBase:: setTransform ( TransformBase * )
{
}

TransformBase * PlotterBase:: getTransform () const
{
  return 0;
}

bool PlotterBase::hasAutoScaled () const
{
  return false;
}

ProjectorBase * PlotterBase::activeProjector () const
{
  return 0;
}

ProjectorBase *
PlotterBase::
getProjector ( int ) const
{
  return 0;
}

int PlotterBase::getNumDataReps () const
{
  return 0;
}

DataRep *
PlotterBase::
getDataRep ( int ) const
{
  return 0;
}

DataRep * PlotterBase::selectedDataRep ( ) const
{
  return 0;
}

void
PlotterBase::
addDataRep ( DataRep * )
{
}

void
PlotterBase::
setActivePlot ( int, bool )
{
  // Only CompositePlotter responds to this.
}

int PlotterBase::activePlotIndex ( ) const
{
  return 0;
}

void PlotterBase::removeDataRep ( DataRep * )
{
  // Only the composite plotters respond.
}

const BinToColor *
PlotterBase::
getValueRep () const
{
  return NULL;
}

void
PlotterBase::
setValueRep ( BinToColor * )
{
  // do nothing
}

void
PlotterBase::
setAutoRanging ( const std::string & axis, bool flag )
{
  Axes::Type at = Axes::convert ( axis );
  setAutoRanging ( at, flag );

  autoScale ();
}

/* @bug @@@ When the histogram weight is set using the Inspector, this
   assert fails and the program crashes.  
   
   @request @@@ Could we change all asserts of this to type (I count
   about 80 of them throughout the code) to tests which throw a
   run-time error exception instead?
*/
void
PlotterBase::
setAutoRanging ( hippodraw::Axes::Type, bool )
{
}

/* virtual dummy holder for non-composite displays */
void
PlotterBase::
setAutoRanging ( bool )
{
}

bool
PlotterBase::
isAutoRanging ( hippodraw::Axes::Type ) const
{
  return false;
}

void
PlotterBase::
setReverse ( bool )
{
}

bool
PlotterBase::
isReverse ( ) const
{
  return false;
}

void PlotterBase::setTitle( const std::string & title )
{
  if ( title != "%t" ) {
    m_title = title;
  } else {
    int index = activePlotIndex ();
    if ( index > -1 ) {
      if ( index == -1 ) index = 0;
      DataRep * rep = getDataRep ( index );
      // could be zero during initialization from document file
      if ( rep != 0 ) rep -> setTitle ( title );
    }
  }

  notifyObservers ();
}

const string & PlotterBase::getTitle() const
{
  if ( m_title != "%t" ) {
    return m_title;
  }
  int index = activePlotIndex ();
  if ( index  < 0 ) index = 0;
  const DataRep * rep = getDataRep ( index );

  return rep -> getTitle ();
}

void PlotterBase::setLabel ( const std::string & axis, 
			     const std::string & label )
{
  Axes::Type type = Axes::convert ( axis );
  setLabel ( type, label );

  notifyObservers ();
}

void
PlotterBase::
setLabel ( hippodraw::Axes::Type, const std::string & )
{
}

const string &
PlotterBase::
getLabel ( hippodraw::Axes::Type ) const
{
  static string s;

  return s;
}

const string &
PlotterBase::
getInternalTitle ( ) const
{
  return m_title;
}

const string &
PlotterBase::
getInternalLabel ( hippodraw::Axes::Type ) const
{
  static string s;

  return s;
}

void PlotterBase::autoScale ( )
{
// do nothing
}

void 
PlotterBase::
setRange ( const std::string & axis, double low, double high )
{
  Axes::Type type = Axes::convert ( axis );
  const Range range ( low, high );

  setRange ( type, range, true, true );
}

void
PlotterBase::
setRange ( hippodraw::Axes::Type, const Range &, 
	   bool, bool )
{
}

void PlotterBase::setLowRange ( hippodraw::Axes::Type,
				int, bool )
{

}

void PlotterBase::setHighRange ( hippodraw::Axes::Type,
				 int, bool )
{
}

/** @todo In the future, keep a list of the axis rather than hardwired
    them. */
const Range & 
PlotterBase::
getRange ( hippodraw::Axes::Type, bool ) const
{
  static Range range;

  return range;
}

void
PlotterBase::
setNumberOfBins ( hippodraw::Axes::Type, unsigned int )
{
  // does nothing
}

void 
PlotterBase::
setBinWidth ( hippodraw::Axes::Type, double )
{
  // does nothing
}

void
PlotterBase::
setBinWidth ( Axes::Type axis, int parm, bool dragging )
{
  int index = activePlotIndex ();
  index = std::max ( 0, index );

  ProjectorBase * projector = getProjector ( index );
  const Range & range = projector -> setBinWidth ( axis, parm, dragging );

  setRange ( axis, range, false );
}

void
PlotterBase::
setOffset ( hippodraw::Axes::Type, double )
{
  // does nothing
}

void 
PlotterBase::
setBinWidth ( const std::string & axis, double width )
{
  Axes::Type at = Axes::convert ( axis );
  setBinWidth ( at, width );
}

void 
PlotterBase::
setOffset ( const std::string & axis, double offset )
{
  Axes::Type at = Axes::convert ( axis );
  setOffset ( at, offset );
}

void 
PlotterBase::
setNumberOfBins ( const std::string & axis, unsigned int number )
{
  Axes::Type at = Axes::convert ( axis );
  setNumberOfBins ( at, number );
}

void 
PlotterBase::
reset ( )
{
  // does nothing
}

const Rect &
PlotterBase::
getUserRect() const
{
  return m_user_rect;
}

const Rect &
PlotterBase::
getRawRect() const
{
  return m_raw_rect;
}

void
PlotterBase::
setScaleFactor ( hippodraw::Axes::Type, double )
{
}

void
PlotterBase::
setScaling ( hippodraw::Axes::Type, bool )
{
}

double
PlotterBase::
getBinWidth ( const std::string & axis ) const
{
  Axes::Type type = Axes::convert ( axis );

  return getBinWidth ( type );
}

double
PlotterBase::
getBinWidth ( hippodraw::Axes::Type ) const
{
  return 1.0;
}

double
PlotterBase::
getOffset ( hippodraw::Axes::Type ) const
{
  return 1.0;
}

double
PlotterBase::
getOffset ( const std::string & axis ) const
{
  Axes::Type type = Axes::convert ( axis );

  return getOffset ( type );
}

int
PlotterBase::
getNumberOfEntries () const
{
  return 0;
}

int
PlotterBase::
getNumOfEntries () const
{
  return getNumberOfEntries ();
}

void PlotterBase::checkAxisScaling ( )
{
// do nothing
}

double
PlotterBase::
getPosRange ( hippodraw::Axes::Type ) const
{
  return DBL_EPSILON;
}

double PlotterBase::getZValue ( double , double , bool ) const
{
  return 0.0;
}

void PlotterBase::setCrossX ( double val )
{
  m_crossX = val;
}

void PlotterBase::setCrossY ( double val )
{
  m_crossY = val;
}

void
PlotterBase::
drawCrossHairs ( ViewBase * )
{
  // does nothing
}

void
PlotterBase::
addValues ( const std::vector < double > & )
{
  // do nothing
}

double
PlotterBase::
getAspectRatio () const
{
  return m_aspect_ratio;
}

void
PlotterBase::
setAspectRatio ( double ratio )
{
  m_aspect_ratio = ratio;
  notifyObservers ();
}

bool
PlotterBase::
wantsPixmap ( ) const
{
  return false;
}

NTuple *
PlotterBase::
createPickTuple ()
{
  return 0;
}

void
PlotterBase::
fillPickedPointFrom ( double, double, 
		      std::vector < double > & ) const
{
}

void PlotterBase::setOffsets( double xoffset, double yoffset )
{
  setOffset ( Axes::X, xoffset );
  setOffset ( Axes::Y, yoffset );
}

int PlotterBase::getParentDataRepIndex ( ) const
{
  return m_parent_datarep_index;
}
  
void PlotterBase::setParentDataRepIndex ( int index ) 
{
  m_parent_datarep_index = index;
}

PlotterBase* PlotterBase::getParentPlotter ( ) const
{
  return m_parent_plotter;
}
  
void PlotterBase::setParentPlotter( PlotterBase* plotter ) 
{
  m_parent_plotter = plotter;
}

void
PlotterBase::
setEnableZ ( bool )
{
  // do nothing
}

void
PlotterBase::
setAutoTicks ( hippodraw::Axes::Type, bool )
{
  // do nothing
}

void
PlotterBase::
setTicks ( hippodraw::Axes::Type, const std::vector < AxisTick > & )
{
}

void
PlotterBase::
setAutoTicksOnX ( bool yes )
{
  setAutoTicks ( Axes::X, yes );
}

void
PlotterBase::
setAutoTicks ( const std::string & axis, bool yes )
{
  Axes::Type type = Axes::convert ( axis );
  setAutoTicks ( type, yes );
}

void
PlotterBase::
setTicksOnX ( const std::vector < double > & values,
	      const std::vector < std::string > & labels )
{
  unsigned int size = values.size ();
  vector < AxisTick > ticks ( size );

  for ( unsigned int i = 0; i < size; i++ ) {
    ticks[i].setValue ( values[i] );
    ticks[i].setContent ( labels[i] );
  }

  setTicks ( Axes::X, ticks );
}

void
PlotterBase::
setTicks ( const std::string & axis, 
	   const std::vector < double > & values,
	   const std::vector < std::string > & labels )
{
  unsigned int size = values.size ();
  vector < AxisTick > ticks ( size );

  for ( unsigned int i = 0; i < size; i++ ) {
    ticks[i].setValue ( values[i] );
    ticks[i].setContent ( labels[i] );
  }
  Axes::Type type = Axes::convert ( axis );

  setTicks ( type, ticks );
}

double
PlotterBase::
getLowRangeOnX () const
{
  const Range & range = getRange ( Axes::X, true ); // scaled range

  return range.low ();
}

double
PlotterBase::
getHighRangeOnX () const
{
  const Range & range = getRange ( Axes::X, true ); // scaled range

  return range.high ();
}

void
PlotterBase::
matrixTranspose ( bool )
{
  // do nothing
}

bool
PlotterBase::
isTargetable () const
{
  return false;
}

DataRep *
PlotterBase::
getTarget () const
{
  return 0;
}

int
PlotterBase::
indexOf ( const DataRep * ) const
{
  return -1;
}

void
PlotterBase::
toggleActivePlot ()
{
  int to_select = 0;
  int number = getNumDataReps ();

  if ( number > 1 ) {
    to_select = activePlotIndex ();
    to_select++;

    if ( to_select == number ) {
      to_select = -1;
    }
    setActivePlot ( to_select, true );
  }
}

void 
PlotterBase::
setPlotterId ()
{
   m_plotterId = s_plotters;
   s_plotters++;
}

int
PlotterBase::
plotterId () const 
{
   return m_plotterId;
}

int 
PlotterBase::
saveView ( const std::vector<double> & range_values ) 
{
   if (range_values.size() != 4) {
      throw PlotterException("PlotterBase::saveView: "
                             "Invalid number of range values.");
   }
   m_views.push_back(range_values);
   
   m_currentView = static_cast<int>(m_views.size() - 1);

   return m_currentView;
}

int 
PlotterBase::
saveViewAtIndex ( const std::vector<double> & range_values , unsigned int i ) 
{
  if ( i > m_views.size() ) {
    i = 0;
  }
 
   if (range_values.size() != 4) {
      throw PlotterException("PlotterBase::saveView: "
                             "Invalid number of range values.");
   }
   vector < vector < double > > :: iterator iter = m_views.begin ();
   advance ( iter, i );
   m_views.insert( iter , range_values);

   for ( unsigned int k = m_views.size() - 1 ; k>i; k--) {
     m_views.pop_back();
   }
   m_currentView = static_cast<int>( i );

   return m_currentView;
}


int 
PlotterBase::
nextView ( bool stepForward )
{
   if (m_views.size() == 0) {
      return -1;
   }
   if (stepForward) {
      m_currentView += 1;
      if (m_currentView >= static_cast<int>(m_views.size())) {
         m_currentView = 0;
      }
   } else {
      m_currentView -= 1;
      if (m_currentView <= -1) {
         m_currentView = m_views.size() - 1;
      }
   }
   setView(m_currentView);
   return m_currentView;
}



void
PlotterBase::
setView ( int index )
{
   if ( m_views.size() == 0 || index < 0 || 
        index >= static_cast<int>(m_views.size()) ) {
      string what ( "PlotterBase::setView: Invalid plot view index " );
      what += String::convert ( index );
      what += ".";
      throw PlotterException ( what );
   }
   setRange("x", m_views[index][0], m_views[index][1]);
   setRange("y", m_views[index][2], m_views[index][3]);
   m_currentView = index;
}

int
PlotterBase::
numViews () const
{
   return static_cast<int>(m_views.size());
}

void
PlotterBase::
deleteView ( int index )
{
   std::vector< std::vector<double> > views;
   for (unsigned int i = 0; i < m_views.size(); i++) {
      if (static_cast<int>(i) != index) {
         views.push_back(m_views[i]);
      }
   }
   m_views = views;
   index--;
   if (index > 0 && m_views.size() > 0) {
      setView(index);
   }
}

int 
PlotterBase::
currentView () const
{
   return m_currentView;
}

void
PlotterBase::
fillCutList ( std::vector < const TupleCut * > & ) const
{
  // do nothing.
}

void
PlotterBase::
setCutRangeAt ( const Range &, unsigned int )
{
  // do nothing
}

void
PlotterBase::
setCurrentRangeSaved( bool flag )
{
  current_range_saved = flag;
}

bool
PlotterBase::
getCurrentRangeSaved( )
{
  return current_range_saved ;
}

Range
PlotterBase::
getDataRange( hippodraw::Axes::Type axis )
{
  ProjectorBase * proj = activeProjector ();
  Range r = proj -> dataRangeOn ( axis );
  return r;
}

const std::vector <double> &
PlotterBase::
getZValues()
{
  ProjectorBase * proj = activeProjector ();
  return ( proj -> getZValues () );
}

const std::vector <unsigned int> &
PlotterBase::
getShape()
{
  ProjectorBase * proj = activeProjector();
  return ( proj -> getShape() );
}

void
PlotterBase::
setShowGrid( bool flag )
{
  
}

void
PlotterBase::
setBoxEdge( bool flag )
{
}

bool
PlotterBase::
getBoxEdge()
{
  return false;
}

bool
PlotterBase::
getShowGrid ()
{
  return false;
}

void 
PlotterBase:: 
setFitsTransform ( const std::string & transform )
{
}

TransformBase *
PlotterBase::
getFitsTransform () const
{
  return 0;
}

void
PlotterBase::
setMinEntries(int entries)
{
}

int
PlotterBase::
getMinEntries ()
{
  return -1;
}

const FontBase *
PlotterBase::
titleFont( ) const
{
  return NULL;
}

FontBase*
PlotterBase::
labelFont ( hippodraw::Axes::Type axes ) const
{  
  return NULL;
}

bool
PlotterBase::
isImageConvertable () const
{
  return false;
}

bool
PlotterBase::
isTextPlotter() const
{
  return false;
}


double 
PlotterBase::
getTopMargin()
{
  return 0.0;
}

double
PlotterBase::
getZMargin()
{
  return 0.0;
}

double 
PlotterBase::
getBottomMargin()
{
  return 0.0;
}

double
PlotterBase::
getLeftMargin()
{
  return 0.0;
}

void
PlotterBase::
setTopMargin( double top )
{
  // Do nothing
}

void
PlotterBase::
setZMargin( double z )
{
  // Do nothing
}

void
PlotterBase::
setBottomMargin( double bottom )
{
  // Do nothing
}

void
PlotterBase::
setLeftMargin( double left )
{
  // Do nothing
}

void 
PlotterBase::
setNeedUpdate(bool isChanged)
{
  // Do nothing
}

double
PlotterBase::
getScaleFactor ( hippodraw::Axes::Type axis ) const
{
  return 1.0;
}

bool
PlotterBase::
isAxisScaled ( hippodraw::Axes::Type axis ) const
{
  return false;
}
