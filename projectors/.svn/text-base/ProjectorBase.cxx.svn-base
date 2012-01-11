/** @file

hippodraw::ProjectorBase class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ProjectorBase.cxx,v 1.209 2007/06/01 21:45:52 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ProjectorBase.h"
#include "ProjectorHelper.h"

#include "axes/AxisModelBase.h"
#include "datasrcs/DataSource.h"
#include "transforms/BinaryTransform.h"


#include <algorithm>

#include <cassert>

#ifdef ITERATOR_MEMBER_DEFECT
using namespace std;
#else
using std::find;
using std::string;
using std::vector;
using std::distance;
#endif

using namespace hippodraw;

ProjectorBase::ProjectorBase ()
  : m_isDirty ( true ),
    m_proj_values ( 0 ),
    m_x_axis ( 0 ),
    m_y_axis ( 0 ),
    m_z_axis ( 0 )
{
}

ProjectorBase::ProjectorBase ( const ProjectorBase & p )
  : Observable (),
    Observer (),
    m_isDirty ( true ),
    m_proj_values ( 0 ),
    m_x_axis ( p.m_x_axis ),
    m_y_axis ( p.m_y_axis ),
    m_z_axis ( p.m_z_axis )
{
}

ProjectorBase::~ProjectorBase ()
{
  if ( m_proj_values != 0 ) delete m_proj_values;
}

/* virtual */
void ProjectorBase::prepareValues ()
{
  setDirty ( false );
}

bool ProjectorBase::isDirty () const
{
  return m_isDirty;
}

void ProjectorBase::setDirty ( bool value )
{
  m_isDirty = value;

  if ( value == true ) notifyObservers ();
}

void
ProjectorBase::
setAxisBinding ( const std::string &,
		 const std::string & )
{
  assert ( false );
}

void
ProjectorBase::
setAxisBindings ( const std::vector < std::string > & )
{
  assert ( false );
}

const vector < string > &
ProjectorBase::
getAxisBindings () const
{
  assert ( false );
  static vector < string > dummy;

  return dummy;
}

void
ProjectorBase::
matrixTranspose ( bool )
{
  // does nothing
}

int
ProjectorBase::
getNumberOfBins ( hippodraw::Axes::Type ) const
{
  return 0;
}

bool ProjectorBase::isAxisBinned ( const std::string & ) const
{
  return false;
}

bool ProjectorBase::isValueBinned () const
{
  return false;
}

void
ProjectorBase::
setRange ( hippodraw::Axes::Type axis, bool )
{
  // The only supported axis so far...
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::Z ) assert ( m_z_axis != 0 );

  setDirty ( true );
}

const Range &
ProjectorBase::
getRange ( hippodraw::Axes::Type axis ) const
{
  // All the axis supported so far  
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );
  
  if ( axis == Axes::X ) {
    return m_x_axis->getRange (false);
  }
  if ( axis == Axes::Y ) {
    return m_y_axis->getRange (false);
  }
  //else
  assert ( m_z_axis != 0 );
  return m_z_axis->getRange (false);
}

Range
ProjectorBase::
preferredRange ( hippodraw::Axes::Type axis ) const
{
  Range range = dataRangeOn ( axis );

  if ( range.length() == 0.0 ) {

    double low = range.low ();

    if ( axis == Axes::Y && 
	 low == 0.0 ) {
	range.setHigh ( 1.0 );
    }
    else {
      if ( low > 0.0 ) { 
	range.setLow ( 0.9 * low );
	range.setHigh ( 1.1 * low );
      }
      else {
	range.setLow ( 1.1 * low );
	range.setHigh ( 0.9 * low );
      }
    }
  }

  return range;
}

void
ProjectorBase::
setAxisModel ( hippodraw::Axes::Type axis,
	       AxisModelBase * axis_model )
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    m_x_axis = axis_model;
  }
  if ( axis == Axes::Y ) {
    m_y_axis = axis_model;
  }

  if ( axis == Axes::Z ) {
    m_z_axis = axis_model;
  }
}

AxisModelBase *
ProjectorBase::
getAxisModel ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if ( axis == Axes::X ) {
    return m_x_axis;
  }
  
  if ( axis == Axes::Y ) {
    return m_y_axis;
  }

  return m_z_axis;
}

void
ProjectorBase::
setNumberOfBins ( hippodraw::Axes::Type, unsigned int  )
{
}

const Range &
ProjectorBase::
setBinWidth ( hippodraw::Axes::Type, double  )
{
  static Range range ( 0., 1. );
  return range;
}

const Range &
ProjectorBase::
setBinWidth ( Axes::Type,  int, bool )
{
  return m_x_axis -> getRange ( false );

}

void ProjectorBase::setOffset ( const std::string &, 
				int, 
				bool )
{
}

void
ProjectorBase::
setOffset ( hippodraw::Axes::Type, double )
{
}

void 
ProjectorBase::
reset ( )
{
}

double
ProjectorBase::
getOffset ( hippodraw::Axes::Type ) const
{
  return 0.0;
}

/** @todo Resolve if this method should be done exterior to the
    projector class by a functor using iterators.
*/
double
ProjectorBase::
getAverage ( hippodraw::Axes::Type ) const
{
  // Never used as subclasses do the work.
  return 0.0;
}

/** @request Can do better than use only the projected values in some
    derived classes.
 */
double
ProjectorBase::
getRMS ( hippodraw::Axes::Type axis )
{
  ProjectorHelper helper( getProjectedValues() );
  
  double rms = 0.0;
  
  if ( axis == Axes::X ) 
    rms = helper.stdCoord();
  
  return rms;
}

/** @todo Change to fetch labels by AxesType or its replacement. */
const std::string & ProjectorBase::getZLabel () const
{
  return m_z_label;
}

int
ProjectorBase::
indexOf ( const std::string & label ) const
{
  return -1;
}

double
ProjectorBase::
getBinWidth ( hippodraw::Axes::Type ) const
{
  return 0.0;
}

double
ProjectorBase::
getZValue ( double, double ) const
{ 
  return 0.0;
}

void
ProjectorBase::
addValues ( const std::vector < double > & )
{
  // do nothing
}

const vector <string> & ProjectorBase::getPointReps() const
{
  return m_pointreps;
}

NTuple *
ProjectorBase::
getNTupleAfterCuts() const
{
   return 0;
}

void 
ProjectorBase::
fillColumnAfterCuts(const std::string & column,
                    std::vector<double> & columnData) const {
// do nothing by default
}

NTuple * 
ProjectorBase::
createNTupleWith ( const std::vector< TupleCut > & cut_list ) const
{
  return 0;
}


bool
ProjectorBase::
wantsScaleFactor ( const std::string &  ) const
{
  return false;
}

const DataSource * 
ProjectorBase::
getProjectedValues () const
{
  return m_proj_values;
}

const DataSource *
ProjectorBase::
createOldStyleNTuple () const
{
  ProjectorBase * projector = const_cast < ProjectorBase * > ( this );
  projector -> prepareValues ();

  return getProjectedValues ();
}

void
ProjectorBase::
normalizeTo ( double )
{
  // does nothing
}

void
ProjectorBase::
setNormalizing ( bool )
{
  // does nothing
}

void
ProjectorBase::
update ( const Observable * )
{
  // does nothing
}

void
ProjectorBase::
normalizeTo ( const ProjectorBase * )
{
  // does nothing
}

void
ProjectorBase::
checkScaling ()
{
  // does nothing
}

const std::vector <double> &
ProjectorBase::
getZValues()
{
  const DataSource * ds = getProjectedValues ();
  const std::vector <double> & oldzval = ds->getColumn (2);
  const std::vector < unsigned int > & shape = getShape();

  unsigned int x = shape[0];
  unsigned int y = shape[1];
  
  m_zval.clear();
  m_zval.resize ( x*y, 0.0 );

  for ( unsigned int yy = 0; yy<y; yy++ )
    for ( unsigned int xx = 0; xx<x; xx++ )
      m_zval[yy*x+xx] = oldzval[xx*y+yy];

  return m_zval;
}

const std::vector < unsigned int> &
ProjectorBase::
getShape()
{
  const DataSource * ds = getProjectedValues ();
  return ds->getShape ();
}

/** @todo This is the only method in all the classes of this directory
    that needs a heder file from transform directory.  Is there a
    better way?
 */
const std::vector <double> &
ProjectorBase::
getZAfterTransform(TransformBase * transform)
{
  const BinaryTransform * bt 
    = dynamic_cast <const BinaryTransform *>(transform);

  m_zval.clear();
  
  // Lambert 256*256
  if ( bt-> aspectRatio() == 1.0 )
    {
      for ( unsigned int j = 0; j < 256; j++ )
	for ( unsigned int i = 0; i < 256; i++ )
	  {
	    double x= static_cast < double > (i)-128;
	    double y= static_cast < double > (j)-128;
	    double z;

	    bool yes = bt->inverseTransform ( x, y );
	    
	    if ( yes )
	      {
		z = getZValue ( x, y );
	      }
	    else
	      {
		z = 0.0;
	      }

	    m_zval.push_back(z);
	  }
    }
  
  // Hammer 360*180
  else
    {
      for ( unsigned int j = 0; j < 180; j++ )
	for ( unsigned int i = 0; i < 360; i++ )
	  {
	    double x= static_cast < double > (i)-180;
	    double y= static_cast < double > (j)-90;
	    double z;

	    bool yes = bt->inverseTransform ( x, y );
	    
	    if ( yes )
	      {
		z = getZValue ( x, y );
	      }
	    else
	      {
		z = 0.0;
	      }

	    m_zval.push_back(z);
	  }
      }

  return (m_zval);
}

bool
ProjectorBase::
hasDataSourceBindings () const
{
  return false;
}

void
ProjectorBase::
setMinEntries(int entries)
{
}

int
ProjectorBase::
getMinEntries ()
{
  return -1;
}

bool
ProjectorBase::
isImageConvertable () const
{
  return false;
}
