/** @file

hippodraw::DataRep class implementation.

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataRep.cxx,v 1.262 2007/07/20 19:29:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataRep.h"

#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"
#include "projectors/ProjectorBase.h"
#include "reps/CutRangeRep.h"

#include <algorithm>
#include <stdexcept>

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

DataRep::DataRep ( )
  : m_title ( "%t" ),
    m_projector ( 0 ),
    m_rep ( 0 ),
    m_parent_datarep ( 0 ),
    m_cut_rep ( 0 )
{
}

DataRep::DataRep ( const DataRep & rep )
  : Observable ( ),
    Observer (),
    m_name ( rep.m_name ),
    m_title ( rep.m_title ),
    m_cut_rep ( 0 )
{
  m_rep = rep.m_rep->clone ();
  m_projector = rep.m_projector->clone();
  m_parent_datarep = rep.m_parent_datarep;
  if ( rep.m_cut_rep != 0 ) {
    m_cut_rep = rep.m_cut_rep -> clone ();
  }
}

DataRep::~DataRep ()
{
  notifyObservers ( &Observer::willDelete );

  delete m_projector;
  delete m_rep;
  if ( m_cut_rep != 0 ) delete m_cut_rep;
}

void DataRep::update ( const Observable * )
{
  notifyObservers ();
}

const string & DataRep::name () const
{
  return m_name;
}

/* virtual */
DataRep * DataRep::getParentDataRep() const
{
  return m_parent_datarep;
}

/* virtual */
void DataRep::setParentDataRep( DataRep * parent_datarep )
{
  m_parent_datarep =  parent_datarep;
}


bool DataRep:: hasErrorDisplay () const
{
  return false;
}

bool DataRep::hasNTupleBindings () const
{
  return true;
}

void
DataRep::
setAxisBinding ( const std::string & axis, 
		 const std::string & label )
{
  if ( hasNTupleBindings () ) {
    m_projector -> setAxisBinding ( axis, label );
  }
}

void
DataRep::
setAxisBindings ( const std::vector < std::string > & bindings )
{
  if ( hasNTupleBindings () ) {
    m_projector -> setAxisBindings ( bindings );
  }
}

void
DataRep::
matrixTranspose ( bool yes )
{
  m_projector -> matrixTranspose ( yes );
}

void DataRep::setRepSize ( double size )
{
 m_rep->setSize ( size );

 notifyObservers();
}

void DataRep::setDirty ( bool flag )
{
  m_projector->setDirty ( flag );
}

ProjectorBase * DataRep::getProjector ( ) const
{
  return m_projector;
}

void
DataRep::
setAxisModel ( hippodraw::Axes::Type axis, AxisModelBase * model )
{
  m_projector->setAxisModel ( axis, model );
}

void DataRep::setSelected ( bool yes )
{
  if ( yes != isSelected () ) {
    m_rep->setSelected ( yes );
    setDirty ( true );
  }
}

bool
DataRep::
isSelected () const
{
  return m_rep ->isSelected ();
}

bool DataRep::isDirty ( ) const
{
  return m_projector->isDirty();
}

double
DataRep::
getPosRange ( hippodraw::Axes::Type axis )
{
  return m_projector->getPosOn ( axis );
}

Range
DataRep::
preferredRange ( hippodraw::Axes::Type axis ) const
{
  return m_projector->preferredRange ( axis );
}

void
DataRep::
setErrorDisplay ( hippodraw::Axes::Type axis, bool flag )
{
  assert ( axis == Axes::X || axis == Axes::Y );

  m_rep -> setErrorOn ( axis, flag );

  notifyObservers ();
}

bool
DataRep::
isErrorDisplayed ( hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y );


  if ( axis == Axes::X ) {
    return m_rep->xError ( );
  }
  //else
  return m_rep->yError ( );
}


void
DataRep::
setRange ( hippodraw::Axes::Type axis, bool const_width )
{
  m_projector->setRange ( axis, const_width );
}

void DataRep::setPointRep ( RepBase * rep )
{
  Color color ( Color::black );
  if ( m_rep != 0 ) {
    bool yes_x = m_rep -> xError ();
    bool yes_y = m_rep -> yError ();
    color = m_rep -> getColor ();
    delete m_rep;

    rep -> setErrorOn ( Axes::X, yes_x );
    rep -> setErrorOn ( Axes::Y, yes_y );
    if (!(color == Color(Color::black)))
	rep -> setColor ( color );
  }
  m_rep = rep;
  notifyObservers ();
}

RepBase * DataRep::getRepresentation () const
{
  return m_rep;
}

void
DataRep::
set ( Color::Value value )
{
  const Color color ( value );
  m_rep -> setColor ( color );

  notifyObservers ();
}

void DataRep::setRepColor ( const Color & color )
{
  m_rep->setColor ( color );

  notifyObservers ();
}

void DataRep::setRepStyle ( int style )
{
  m_rep->setStyle ( style );
  
  notifyObservers ();
}

const Color & DataRep::getRepColor () const
{
  return m_rep->color ();
}

const DataSource *
DataRep::
getProjectedValues () const
{
  m_projector->prepareValues();
  const DataSource * ntuple = m_projector -> getProjectedValues ();

  return ntuple;
}

void
DataRep::
drawProjectedValues ( TransformBase * transform, ViewBase * view )
{
  drawCuts ( transform, view );
  const DataSource * ntuple = getProjectedValues ();

  m_rep -> drawProjectedValues ( ntuple, transform, view );
}

void
DataRep::
drawCuts ( TransformBase * transform, ViewBase * view )
{
  if ( m_cut_rep != 0 ) {
    const Range & x_range = m_projector -> getRange ( Axes::X );
    const Range & y_range = m_projector -> getRange ( Axes::Y );
    m_cut_rep -> drawProjectedValues ( x_range, y_range, *transform, *view );
  }
}

bool DataRep::acceptFunction ( int )
{
  return false;
}

bool DataRep::hasZeroRows ( )
{
  bool yes = true;
  if ( m_projector != 0 )  {
    yes = m_projector->isEmpty ();
  }

  return yes;
}

bool DataRep::hasZoomY ( ) const
{
  return false;
}

void
DataRep::
addValues  ( const std::vector < double > & v )
{
  if ( hasNTupleBindings () ) return;

  m_projector -> addValues ( v );
}

NTuple *
DataRep::
getNTupleAfterCuts () const
{
   return m_projector->getNTupleAfterCuts();
}

void
DataRep::
fillColumnAfterCuts(const std::string & column,
                    std::vector<double> & columnData) const {
   m_projector->fillColumnAfterCuts(column, columnData);
}

NTuple *
DataRep::
createNTupleUnderRegion () const
{
  const vector < TupleCut > & cut_list = m_cut_rep -> getCuts ();

  return m_projector -> createNTupleWith ( cut_list );
}

double
DataRep::
getMean ( const std::string & axis )
{
  Axes::Type type = Axes::convert ( axis );

  return m_projector -> getAverage ( type );
}

double
DataRep::
getRMS ( const std::string & axis )
{
  Axes::Type type = Axes::convert ( axis );

  return m_projector -> getRMS ( type );
}

bool
DataRep::
isAxisBinned ( hippodraw::Axes::Type axis ) const
{
  bool yes = false;
  if ( axis == Axes::X ) yes = m_projector -> isAxisBinned ( "X" );
  else if ( axis == Axes::Y ) yes = m_projector -> isAxisBinned ( "Y" );

  return yes;
}

void
DataRep::
update ()
{
  if (m_projector != NULL)
	m_projector->setDirty(true);
}

void
DataRep::
 normalizeTo ( const DataRep * target )
{
  ProjectorBase * projector = target -> getProjector ();

  m_projector -> normalizeTo ( projector );
}

const Range &
DataRep::
setBinWidth ( hippodraw::Axes::Type axis, double width )
{
  return m_projector -> setBinWidth ( axis, width );
}

int
DataRep::
getNumberOfEntries () const
{
  return m_projector -> getNumberOfEntries ();
}

bool
DataRep::
isTargetable () const
{
  return true;
}

NTuple *
DataRep::
createNTuple () const
{
  m_projector->prepareValues();
  const DataSource * ds = m_projector -> createOldStyleNTuple ();
  NTuple * ntuple = new NTuple ( ds );

  return ntuple;
}

void
DataRep::
setTitle ( const std::string & title )
{
  m_title = title;
}

const std::string &
DataRep::
getTitle ( ) const
{
  if ( m_title == "%t" ) {
    return m_projector -> getTitle ();
  }
  //  else 
  return m_title;
}

bool
DataRep::
needColorRep () const
{
  return hasAxis ( Axes::Z );
}

bool
DataRep::
hasAxis ( hippodraw::Axes::Type ) const
{
  assert ( false );

  return false;
}

void
DataRep::
addCut ( const TupleCut & cut )
{
  if ( m_cut_rep == 0 ) {
    m_cut_rep = new CutRangeRep ();
  }
  m_cut_rep -> addCut ( cut );

  setDirty ( true );
}

void
DataRep::
removeCut ()
{
  delete m_cut_rep;
  m_cut_rep = 0;
    
  setDirty ( true );
}

void
DataRep::
addCut ( )
{
  TupleCut cut;
  const ProjectorBase * projector = getProjector ();

  const string & label = projector -> getXLabel ();
  cut.setLabel ( label );

  int index = projector -> indexOf ( label );
  cut.setColumn ( index );

  const Range & range  = projector -> getRange ( Axes::X );
  cut.setRange ( range );

  addCut ( cut );
}

const vector < TupleCut > &
DataRep::
getCuts () const
{
  assert ( m_cut_rep != 0 );

  return m_cut_rep -> getCuts ();
}

bool
DataRep::
hasCut () const
{
  return m_cut_rep != 0;
}

void
DataRep::
throwRangeException ()
{
  string what ( "DataRep::setCutRange: index out of range." );
  throw std::out_of_range ( what );
}

void
DataRep::
setCutRangeAt ( const Range & range, unsigned int i )
{
  m_cut_rep -> setCutRangeAt ( range, i );
}

void
DataRep::
toggleInverted ( unsigned int i )
{
  assert ( m_cut_rep != 0 );

  m_cut_rep -> toggleInverted ( i );
}

void
DataRep::
setEnabled ( unsigned int i, bool yes )
{
  m_cut_rep -> setEnabled ( i, yes );
}

RepBase *
DataRep::
getCutRep ()
{
  return m_cut_rep;
}

bool
DataRep::
needsMatrixSet () const
{
  return false;
}

bool
DataRep::
isImageConvertable () const
{
  return m_projector -> isImageConvertable ();
}

