/** @file

hippodraw::DisplayController class implementation

Copyright (C) 2001-2007 The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DisplayController.cxx,v 1.428 2007/07/20 19:29:56 pfkeb Exp $

@bug how come HAVE_CFITSIO but no HAVE_ROOT
*/

// for cfitsio
#ifdef HAVE_CONFIG_H
#include "config.h"
#else

// for truncation warning in debug mode and version
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#endif // HAVE_CONFIG_H

#include <iostream>
#include "DisplayController.h"

#include "DataRepController.h"
#include "FunctionController.h"

#include "axes/AxisModelLog.h"
#include "axes/AxisModelLinear.h"

#include "binners/BinnerAxis.h"
#include "binners/BinnerAxisFactory.h"

#include "datareps/DataRepException.h"
#include "datareps/DataRepFactory.h"
#include "datareps/FunctionRep.h"
#include "datareps/LineDataRep.h"
#include "datareps/TextDataRep.h"
#include "datareps/StripChart.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTuple.h"
#include "datasrcs/NTupleController.h"

#include "graphics/ViewBase.h"
#include "graphics/ViewFactory.h"

#include "plotters/PlotterException.h"
#include "plotters/PlotterFactory.h"
#include "plotters/TextPlotter.h"
#include "plotters/XyPlotter.h"

#include "projectors/BinningProjector.h"
#include "projectors/NTupleProjector.h"

#include "colorreps/BinToMovingBrkPt.h"
#include "colorreps/BinToColorScaleMovingBrkPt.h"
#include "colorreps/BinToColorFactory.h"
#include "colorreps/BinToUsrDefinedScale.h"
#include "reps/ColumnPointRep.h"
#include "reps/LinePointRep.h"
#include "reps/PointRepFactory.h"
#include "reps/SymbolPointRep.h"
#include "reps/TextRepBase.h"
#include "reps/TextRepFactory.h"

#include "transforms/TransformFactory.h"
#include "transforms/XYTransform.h"

#ifdef HAVE_CFITSIO
#include "fits/FitsController.h"
#endif

#include <algorithm>

#include <cassert>

using std::list;
using std::string;
using std::swap;
using std::vector;
using std::find;

using namespace hippodraw;

DisplayController * DisplayController::s_instance = 0;

DisplayController::DisplayController ( )
{
}

DisplayController::~DisplayController ( )
{
  DataRepController * controller = DataRepController::instance ();
  delete controller;
}

DisplayController * DisplayController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new DisplayController ( );
  }
  return s_instance;
}

const vector < string > &
DisplayController::
getValueTransformTypes () const 
{
BinToColorFactory * factory = BinToColorFactory::instance ();

return factory -> names ();
}

void
DisplayController::
setValueTransform ( PlotterBase * plotter, const std::string & name )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  BinToColor * model = factory -> create ( name );

  plotter -> setValueRep ( model );
}

void
DisplayController::
setValueTransform ( PlotterBase * plotter, int index )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  const vector < string > & names = factory -> names ();
  BinToColor * rep = factory -> create ( names[index] );

  plotter -> setValueRep ( rep );
}

void
DisplayController::
addValueTransform ( const std::string & name,
		    const std::vector < double > & ctrl_points )
{
  BinToColor * model = new BinToUsrDefinedScale ( name.c_str() );
  model -> setControlPoints ( ctrl_points );

  BinToColorFactory * factory = BinToColorFactory::instance ();
  factory -> add ( model );
}

bool
DisplayController::
removeValueTransform ( const std::string & name )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  BinToColor * prototype = factory -> prototype ( name );
  bool yes = prototype -> isUserDefined ();
  if ( yes ) {
    factory -> remove ( name );
  }

  return yes;
}

const vector <double > &
DisplayController::
getValueCtrlPts ( const PlotterBase * plotter )
{
  BinToColor * r = const_cast< BinToColor *> (plotter->getValueRep());
  return r -> getControlPoints ();
}

void
DisplayController::
setValueCtrlPts ( PlotterBase *plotter,
		  const std::vector < double > & sv)
{
  BinToColor * r = const_cast< BinToColor *> (plotter->getValueRep());
  if ( r != 0 ) {
    r -> setControlPoints ( sv );
    DataRep * datarep = plotter -> selectedDataRep ();
    datarep -> notifyObservers ();
  }
}

void
DisplayController::
saveValueCtrlPts( PlotterBase *plotter,
		  const std::vector <double> & sv)
{
  BinToColor * r = const_cast< BinToColor *> (plotter->getValueRep());
  if ( r != 0 ) {
    const std::string transformName = r->name();
    removeValueTransform(transformName);
    addValueTransform(transformName, sv);
  }
}


int
DisplayController::
getValueTransformIndex ( PlotterBase * plotter )
{
  int index = -1;
  const BinToColor * rep = plotter -> getValueRep (); // return the value to color representation
  if ( rep != 0 ) { // may not have one.
    const string & name = rep -> name ();
    const vector < string > & names = getValueTransformTypes ();
    unsigned int size = names.size();
    for ( unsigned int i = 0; i < size; i++ ) {
      if ( name == names[i] ) {
	index = i;
	break;
      }
    }
  }

  return index;
}

bool
DisplayController::
hasControlPoints ( const PlotterBase * plotter ) const
{
  bool yes = false;
  const BinToColor * rep = plotter -> getValueRep ();
  if ( rep != 0 ) { // may not have one
    yes = rep -> hasControlPoints ();
  }

  return yes;
}

bool
DisplayController::
isUserDefinedValueTransform ( const PlotterBase * plotter ) const
{
  bool yes = false;
  const BinToColor * rep = plotter -> getValueRep ();
  if ( rep != 0 ) {
    yes = rep -> isUserDefined ();
  }

  return yes;
}

const vector < string > &
DisplayController::
getDisplayTypes () const
{
  DataRepFactory * factory = DataRepFactory::instance ();

  return factory -> names ();
}

PlotterBase *
DisplayController::
createDisplay ( const std::string & name )
{
  DataRepController * controller = DataRepController::instance ();
  DataRep * rep = controller -> createDataRep ( name );

  return createDisplay ( rep );
}

PlotterBase *
DisplayController::
createDisplay ( DataRep * rep )
{
  bool yes = rep -> hasNTupleBindings ();
  if ( yes ) {
    string what ( "DisplayController: data rep of type " );
    what += rep -> name();
    what +="\nhas NTuple bindings.\n";
    what += "Can not create without them";
    throw DataRepException ( what );
  }

  PlotterBase * plotter = createPlotter ( rep );
  plotter -> addDataRep ( rep );
  plotter -> setAutoRanging ( true );

  return plotter;
}

/** @bug If Image DataRep comes from file other than FITS file, then
    it is not being handled.
 */
PlotterBase *
DisplayController::
createDisplay ( const std::string & name,
		const DataSource & tuple,
		const std::vector< std::string > & bindings ) const
{
  // Expansion needed by RootNTuple with multi-dimensional array
  tuple.expandIfNeeded(bindings);

  DataRepFactory * factory = DataRepFactory::instance ();
  DataRep * proto = factory->prototype ( name );
  assert ( proto != 0 );

  PlotterBase * plotter = createPlotter ( proto );
  addDataRep ( plotter, name, &tuple, bindings );
#ifdef HAVE_CFITSIO
  if ( proto -> needsMatrixSet () ) {
    FitsController * controller = FitsController::instance ();
    controller -> checkForImage ( plotter, tuple );
  }
#endif

  plotter -> autoScale ();

  return plotter;
}

NTuple *
DisplayController::
createNTupleDiff ( PlotterBase * plotter )
{
  NTuple * ntuple = 0;
  ntuple = plotter -> createNTuple ();
  unsigned int size = ntuple -> rows ();

  vector < double > values ( size );
  vector < double > residuals ( size );
  DataRep * data1 = plotter -> getDataRep(0);
  DataRep * data2 = plotter -> getDataRep(1);
  NTuple * ntuple1 = data1 -> createNTuple();
  NTuple * ntuple2 = data2 -> createNTuple();

  vector < double > & x1 = ntuple1 -> getColumn ( 0 ); // X coordinate
  vector < double > & y1 = ntuple1 -> getColumn ( 1 ); // Y coordinate
  vector < double > & deltaX1 = ntuple1 -> getColumn (2); // Half bin width 
 
  vector < double > & x2 = ntuple2 -> getColumn ( 0 ); // X coordinate
  vector < double > & y2 = ntuple2 -> getColumn ( 1 ); // Y coordinate
  vector < double > & deltaX2 = ntuple2 -> getColumn (2); // Half bin width 

  vector <double> xDiff;
  vector <double> yDiff;
  vector <double> deltaXDiff;
  vector <double> deltaYDiff;

  unsigned int i = 0;
  unsigned int j = 0;
  while( i < x1.size() && j < x2.size() ) {  // Compare of two histogram 
    if (x1[i] + deltaX1[i] == x2[j] + deltaX2[j]) {
      if ( xDiff.empty()) {
        xDiff.push_back(x1[i]);
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(deltaX1[i]);
        i++; 
        j++; 
        continue;
      } else {
        double tmpX = xDiff.back();
        double tmpDeltaX = deltaXDiff.back();
        double xLeft = tmpX + tmpDeltaX;
        double xRight = x1[i] + deltaX1[i];
        xDiff.push_back(xLeft+0.5*(xRight - xLeft));
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(0.5*(xRight - xLeft));
        i++; 
        j++; 
        continue;
      }  
    } 
    else if (x1[i] + deltaX1[i] < x2[j] + deltaX2[j]) {
      if ( xDiff.empty()) {
        xDiff.push_back(x1[i]);
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(deltaX1[i]);
        i++; 
        continue;
      } else {
        double tmpX = xDiff.back();
        double tmpDeltaX = deltaXDiff.back();
        double xLeft = tmpX + tmpDeltaX;
        double xRight = x1[i] + deltaX1[i];
        xDiff.push_back(xLeft+0.5*(xRight - xLeft));
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(0.5*(xRight - xLeft));
        i++; 
        continue;
      }  
    } else {
      if ( xDiff.empty() ) {
        xDiff.push_back(x2[j]);
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(deltaX2[j]);
        j++;
        continue;
      } else {
        double tmpX = xDiff.back();
        double tmpDeltaX = deltaXDiff.back();
        double xLeft = tmpX + tmpDeltaX;
        double xRight = x2[j] + deltaX2[j];
        xDiff.push_back(xLeft+0.5*(xRight - xLeft));
        yDiff.push_back(y2[j] - y1[i]);
        deltaXDiff.push_back(0.5*(xRight - xLeft));
        j++;
        continue;
      }
    }
  }

  NTuple *  newNtuple = new NTuple();
  newNtuple->addColumn ( "X", xDiff );
  newNtuple->addColumn ( "Residuals", yDiff );
  newNtuple->addColumn ( "DeltaX", deltaXDiff );

  return newNtuple;
}

PlotterBase *
DisplayController::
createDifferenceDisplay ( PlotterBase * plotter )
{
  NTuple * ntuple = createNTupleDiff ( plotter );
  int size = ntuple->rows();

  ntuple -> setTitle ( plotter -> getTitle () );

  DataSourceController::instance () -> registerNTuple ( ntuple );

  vector < string > bindings ( 3 );

  bindings[0] = ntuple -> getLabelAt ( 0 );
  bindings[1] = ntuple -> getLabelAt ( 1 );
  bindings[2] = ntuple -> getLabelAt ( 2 );

  DisplayController * controller = DisplayController::instance ();

// Make scaling of x-axis match that of the original plot.
  PlotterBase * new_plotter = controller -> createDisplay ( "XY Plot",
                                                            *ntuple,
                                                            bindings );
  controller->setLog( new_plotter, "x",
                      controller -> getLog( plotter, "x" ) );

  return new_plotter;
}


PlotterBase * DisplayController::createPlotter ( const DataRep * rep ) const
{
  PlotterFactory * factory = PlotterFactory::instance ();
  PlotterBase * plotter = 0;
  try {
    plotter = factory -> create ( "XyPlotter" );
    if ( rep -> hasAxis ( Axes::Z ) ) {
      plotter -> setEnableZ ( true );
    }
  }
  catch ( const FactoryException & ) {
    assert ( false );
  }
return plotter;
}

/** @todo If new DataRep is incompatible with exiting Plotter, one
    could create new plotter type. */
DataRep *
DisplayController::
addDataRep ( PlotterBase * plotter,
	     const std::string & type,
	     const DataSource * tuple,
	     const std::vector < std::string > & bindings ) const
{
  bool yes = isCompatible ( plotter, type );
  if ( ! yes ) return 0;

  DataRepController * controller = DataRepController::instance ();
  DataRep * rep = controller->createDataRep ( type, tuple, bindings );
  addDataRep ( plotter, rep );

  return rep;
}

void
DisplayController::
stackDataRep ( PlotterBase * plotter, DataRep * rep ) const
{
  XyPlotter * xyplotter = dynamic_cast < XyPlotter * > ( plotter );
  xyplotter -> addDataRepStacked ( rep );
}

DataRep *
DisplayController::
addDataRepStacked ( PlotterBase * plotter,
		    const std::string & type,
		    const DataSource * tuple,
		    const std::vector < std::string > & bindings ) const
{
  bool yes = isCompatible ( plotter, type );
  if ( ! yes ) return 0;

  DataRepController * controller = DataRepController::instance ();
  DataRep * rep = controller->createDataRep ( type, tuple, bindings );

  stackDataRep ( plotter, rep );

  return rep;
}

LineDataRep *
DisplayController::
addLineRep ( PlotterBase * plotter, hippodraw::Axes::Type axis, double value )
{
  LineDataRep * rep = new LineDataRep ( axis, value );
  addDataRep ( plotter, rep );

  return rep;
}

LineDataRep *
DisplayController::
addLineRep ( PlotterBase * plotter, const std::string & axis, double value )
{
  Axes::Type type = Axes::convert ( axis );

  return addLineRep ( plotter, type, value );
}

void
DisplayController::
fixLogIfBinned ( const PlotterBase * plotter, DataRep * rep ) const
{
  setBinner ( plotter, rep, Axes::X );
  setBinner ( plotter, rep, Axes::Y );
}

/** @bug Shouldn't need to call checkAxisScaling here since the
    CompositePlotter has already done it.  Remove call and test. 
*/
void
DisplayController::
addDataRep ( PlotterBase * plotter,  DataRep * rep ) const
{
  if ( plotter -> getNumDataReps () > 0 ) {
     fixLogIfBinned ( plotter, rep );
  }

  plotter->addDataRep ( rep );

  if ( !rep->hasZeroRows() ){
       plotter->checkAxisScaling ( );
  }

}

/** @bug Part of being compatible is same aspect ratio or none.
 */
bool 
DisplayController::
isCompatible ( const PlotterBase * plotter,
	       const std::string & datarep ) const
{
  DataRepFactory * factory = DataRepFactory::instance ();
  DataRep * proto = factory->prototype ( datarep );

  bool yes = true;
  // does it need Z axis display
  if ( proto -> hasAxis ( Axes::Z ) ) {
    yes = plotter->hasAxis ( Axes::Z );
  }

  return yes;
}

void
DisplayController::
removeTextObservers ( const std::vector < const ViewBase * > & views )
{
#ifdef ITERATOR_MEMBER_DEFECT
  std:: 
#endif
  vector < const ViewBase * > ::const_iterator iter = views.begin();
  
  while ( iter != views.end() ) {

    // If its a text view, tell the target DataRep (observable) to remove
    // this->plotter from its list of observers.

    const ViewBase * curview = ( * iter++ );
    PlotterBase * curplotter = curview->getPlotter ();

    TextPlotter * textPlotter 
      = dynamic_cast < TextPlotter * > ( curplotter );

    if ( textPlotter ){
      const DataRep * target = textPlotter->getParentDataRep();
      if ( target ){
	DataRep * tar = const_cast < DataRep * > ( target );
	tar->removeObserver ( textPlotter );
      }
    }
  }
}

void
DisplayController::
addTextViewToList ( std::vector < const ViewBase * > & dest,
		    const std::vector < const ViewBase * > & src )
{
  std::vector < const ViewBase * > ::const_iterator first = src.begin();
  while ( first != src.end() ) {
    const ViewBase * view = *first++;

    vector < const ViewBase * > :: iterator i 
      = find ( dest.begin(), dest.end (), view );
    if ( i != dest.end() ) continue; // already in list

    PlotterBase * plotter = view->getPlotter ();
    TextPlotter * text_plotter = dynamic_cast < TextPlotter * > ( plotter );
    if ( text_plotter == 0 ) continue; // not a text plotter

    const DataRep * datarep = text_plotter->getParentDataRep ();
    bool add_to_list = false;
    for ( i = dest.begin (); i !=  dest.end(); ++i ) {
      const ViewBase * view = *i;
      PlotterBase * plotter = view->getPlotter ();
      if ( plotter == 0 ) continue; // view has no plotter, can be groupview
      int num = plotter->getNumDataReps ();
      for ( int j = 0; j < num; j++ ) {
	DataRep * rep = plotter->getDataRep ( j );
	if ( rep == datarep ) { 
	  add_to_list = true;
	  break;
	}
      }
      if ( add_to_list ) break;
    }
    if ( add_to_list ) dest.push_back ( view );
  }
}

bool 
DisplayController::
isAxisBinned (  PlotterBase * plotter,
		const std::string & axis )
{
  bool yes = false;

  int index = plotter->activePlotIndex ( );
  if ( index < 0 ) {
    FunctionController * controller = FunctionController::instance();
    index = controller->getUniqueNonFunctionIndex ( plotter );
  }
  if ( index >=  0 ) {
    ProjectorBase * projector = plotter->activeProjector ();
    yes = projector->isAxisBinned ( axis );
  }

  return yes;
}

ViewBase * DisplayController::createTextView ( const ViewFactory * v_factory,
					       PlotterBase * plotter,
					       const std::string & name,
					       const std::string & text)
{
  int index = activeDataRepIndex ( plotter );
  DataRep * datarep = plotter->getDataRep ( index );
  ViewBase * view = createTextView ( v_factory, datarep, name, text );
  assert ( view != 0 );

  // Set the parent plotter of the text plotter.
  view->getPlotter()->setParentPlotter(plotter);

  return view;
}

const vector < string > & 
DisplayController::
getTextTypes () const
{
  TextRepFactory * text_factory = TextRepFactory::instance ();

  return text_factory->names ();
}

DataRep * DisplayController::createTextDataRep ( const std::string & name,
						 const std::string & text)
{
  TextRepFactory * text_factory = TextRepFactory::instance ();
  RepBase * textrep = text_factory->create ( name );
  assert ( textrep != 0 );

  textrep->setText ( text );
  DataRep * datarep = new TextDataRep ( textrep );

  return datarep;
}

PlotterBase * 
DisplayController::createTextPlotter ( DataRep * datarep, 
				       const std::string & name,
				       const std::string & text)
{
  DataRep * textdatarep = createTextDataRep ( name, text );
  TextPlotter * plotter = new TextPlotter ( );
  plotter->addDataRep ( textdatarep );
  plotter->setParentDataRep ( datarep );

  return plotter;
}

ViewBase * DisplayController::createTextView ( const ViewFactory * v_factory,
					       DataRep * datarep,
					       const std::string & name,
					       const std::string & text)
{
  PlotterBase * plotter = createTextPlotter ( datarep, name, text );

  return v_factory->createView ( plotter );
}

int DisplayController::
activeDataRepIndex ( const PlotterBase * plotter ) const
{
  int index = plotter->activePlotIndex ();
  if ( index < 0 ) {
    FunctionController * controller = FunctionController::instance ();
    index = controller->getUniqueNonFunctionIndex ( plotter );
  }
  return index;
}

DataRep *
DisplayController::
activeDataRep ( const PlotterBase * plotter ) const
{
  DataRep * rep = 0;
  int index = activeDataRepIndex ( plotter );
  if ( index >= 0 ) {
    rep = plotter -> getDataRep ( index );
  }

  return rep;
}

void
DisplayController::
setActiveDataRepIndex ( PlotterBase * plotter, int index )
{
  plotter -> setActivePlot ( index, true );
}

const vector < string > & 
DisplayController::
axisBindings ( const PlotterBase * plotter ) const
{
  //  This only make sense for a NTupleProjector, so we downcast
  NTupleProjector * ntProjector =
    dynamic_cast <NTupleProjector *> ( plotter->activeProjector() );

  if ( ntProjector == 0 ) return m_null_vector;

  return ntProjector->getAxisBindings ();
}

const vector < string > &
DisplayController::bindingOptions ( const PlotterBase * plotter,
				    int index )
{
  DataRep * rep = plotter->getDataRep ( index );
  DataRepController * controller = DataRepController::instance ();

  return controller->bindingOptions ( rep );
}

const vector < string > &
DisplayController::
bindingOptions ( const std::string & type )
{
  DataRepFactory * factory = DataRepFactory::instance ();
  DataRep * rep = factory -> prototype ( type );
  DataRepController * controller = DataRepController::instance ();
  
  return controller -> bindingOptions ( rep );
}

const vector < string > & 
DisplayController::
axisBindings ( const PlotterBase * plotter, int index ) const
{
  assert ( index < plotter->getNumDataReps () );

  ProjectorBase * pbase = plotter->getProjector ( index );
  //  This only make sense for a NTupleProjector, so we downcast
  NTupleProjector * ntProjector =
    dynamic_cast <NTupleProjector *> ( pbase );

  if ( ntProjector == 0 ) return m_null_vector;

  return ntProjector->getAxisBindings ();
}

int
DisplayController::
getNumberOfEntries ( const PlotterBase * plotter, int index ) const
{
  assert ( index < plotter->getNumDataReps () );

  ProjectorBase * projector = plotter -> getProjector ( index );

  return projector -> getNumberOfEntries ();
}

int
DisplayController::
getUnderflow ( const PlotterBase * plotter, int index ) const
{
  assert ( index < plotter->getNumDataReps () );
  ProjectorBase * projector = plotter -> getProjector ( index );
  return projector -> getUnderflow ();
}

int
DisplayController::
getOverflow ( const PlotterBase * plotter, int index ) const
{
  assert ( index < plotter->getNumDataReps () );
  ProjectorBase * projector = plotter -> getProjector ( index );
  return projector -> getOverflow ();
}

double
DisplayController::
getAverage ( const PlotterBase * plotter, 
	     const std::string & axis, int index ) const
{
  Axes::Type a = Axes::convert ( axis );

  return getAverage ( plotter, a, index );
}

double
DisplayController::
getAverage ( const PlotterBase * plotter,
	     hippodraw::Axes::Type axis,
	     int index ) const
{
  assert ( index < plotter -> getNumDataReps () );

  ProjectorBase * projector = plotter -> getProjector ( index );

  return projector -> getAverage ( axis );
}

double
DisplayController::
getRMS ( const PlotterBase * plotter, 
	 const std::string & axis, int index ) const
{
  Axes::Type a = Axes::convert ( axis );

  return getRMS ( plotter, a, index );
}

double
DisplayController::
getRMS ( const PlotterBase * plotter,
	 hippodraw::Axes::Type axis,
	 int index ) const
{
  assert ( index < plotter -> getNumDataReps () );

  ProjectorBase * projector = plotter -> getProjector ( index );

  return projector -> getRMS ( axis );
}

const string & DisplayController::getType ( const PlotterBase * plotter,
					    int index ) const
{
  DataRep * datarep = plotter->getDataRep ( index );

  return datarep->name ();
}

ProjectorBase * 
DisplayController::getProjector ( const PlotterBase * plotter ) const
{
  DataRep * datarep = plotter->selectedDataRep ();

  if ( datarep == 0 ) return 0;

  return datarep->getProjector ();
}

/** @todo Can replace much of this code by calling
    getUniqueWithNTupleBindings member function. 
*/
NTupleProjector * 
DisplayController::getBindingProjector ( const PlotterBase * plotter ) const
{
  ProjectorBase * projbase = getProjector ( plotter );
  if ( projbase != 0 ) {
    NTupleProjector * projector 
      = dynamic_cast < NTupleProjector * > ( projbase );
    return projector;
  }
  // Failure of the above is because multiple DataRep objects are
  // selected.  However, if the the additional ones are all
  // FunctionRep objects, then we want to find the single DataRep
  // object.
  int number = plotter->getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataRep * rep = plotter->getDataRep ( i );
    FunctionRep * frep = dynamic_cast < FunctionRep * > ( rep );
    if ( frep != 0 ) continue;
    if ( projbase == 0 ) { 
      projbase = rep->getProjector ();
    }
    else {
      return 0; // multiple non- FunctionRep objects found
    }
  }
  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( projbase );

  return projector;
}

void DisplayController::setNTuple ( PlotterBase * plotter,
				    const NTuple * ntuple ) const
{
  NTupleProjector * projector = getBindingProjector ( plotter );
  if ( projector == 0 ) return;

  projector->setNTuple ( ntuple );
  NTuple * nt = const_cast < NTuple * > ( ntuple );
  nt->addObserver ( projector );
  DataRep * datarep = plotter->selectedDataRep ();

  datarep->notifyObservers ();
}
DataSource *
DisplayController::
getDataSource ( const PlotterBase * plotter )
{
  DataSource * source = 0;
  int index = activeDataRepIndex ( plotter );
  int numDataReps = plotter -> getNumDataReps ();
  if ( index >= 0 || numDataReps < 2 ) {
    source = getDataSource ( plotter, 0 );
  }
  else {
    source = getDataSource ( plotter, 0 );
    for ( int i = 1; i< numDataReps; i++ ) {
      const DataSource * nt = getDataSource ( plotter, i );
      if ( source != nt ) {
	source = 0;
	break;
      }
    }
  }
  return source;
}

DataSource * 
DisplayController::
getDataSource ( const PlotterBase * plotter, int index )
{
  DataSource * source = 0;

  DataRep * rep = plotter->getDataRep ( index );
  if ( rep->hasNTupleBindings () ) {
    ProjectorBase * pbase = rep->getProjector ();
    NTupleProjector * projector = dynamic_cast < NTupleProjector * > ( pbase );
    if ( projector != 0 ) {
      source = projector -> getNTuple ();
    }
  }

  return source;
}

void
DisplayController::
setIntervalCount ( const PlotterBase * plotter, unsigned int count )
{
  int i = activeDataRepIndex ( plotter );
  if ( i < 0 ) return;
  DataSource * ds = getDataSource ( plotter, i );
  NTuple * ntuple = dynamic_cast < NTuple * > ( ds );

  if ( ntuple != 0 ) ntuple->setIntervalCount ( count );
}

void
DisplayController::
setIntervalEnabled ( const PlotterBase * plotter, bool yes )
{
  int i = activeDataRepIndex ( plotter );
  if ( i < 0 ) return;
  DataSource * ds = getDataSource ( plotter, i );
  NTuple * ntuple = dynamic_cast < NTuple * > ( ds );

  if ( ntuple != 0 ) ntuple->setIntervalEnabled ( yes );
}

void
DisplayController::
setAllIntervalEnabled ( const PlotterBase * plotter, bool yes )
{
  int number = plotter->getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataSource * ds = getDataSource ( plotter, i );
    NTuple * ntuple = dynamic_cast < NTuple * > ( ds );
    if ( ntuple != 0 ) ntuple->setIntervalEnabled ( yes );
  }
}

bool
DisplayController::
areDataSourcesSaved ( const PlotterBase * plotter )
{
  bool yes = true;
  DataSourceController * controller = DataSourceController::instance ();

  int number = plotter->getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    const DataSource * source = getDataSource ( plotter, i );
    if ( source != 0 ) {
      yes = controller -> isFromFile ( source );
    }
    if ( yes == false ) break;
  }

  return yes;
}

bool DisplayController::hasNTupleBindings ( const PlotterBase * plotter,
					    int index )
{
  assert ( ! ( index < 0 ) );
  DataRep * rep = plotter->getDataRep ( index );

  return rep->hasNTupleBindings ();
}

const string & 
DisplayController::
getDataSourceName ( const PlotterBase * plotter,
		    int index )
{
  const DataSource * source = getDataSource ( plotter, index );

  return source -> getName ();
}

const vector < string > & 
DisplayController::getDataSourceLabels ( const PlotterBase * plotter,
					 int index )
{
  DataRep * datarep = plotter->getDataRep ( index );
  ProjectorBase * pbase = datarep->getProjector ();
  NTupleProjector * projector = dynamic_cast < NTupleProjector * > ( pbase );
  if ( projector == 0 ) return m_null_vector;

  const DataSource * tuple = projector->getNTuple ();

  return tuple->getLabels ();
}

DataRep *
DisplayController::
getUniqueWithNTupleBindings ( const PlotterBase * plotter )
{
  DataRep * rep = 0;
  int active = plotter -> activePlotIndex ();

  if ( active < 0 ) { // multiple active
    int count = 0;
    int number = plotter -> getNumDataReps ();
    for ( int i = 0; i < number; i++ ) {
      DataRep * trep = plotter -> getDataRep ( i );
      if ( trep -> hasNTupleBindings () ) {
	if ( count == 0 ) {
	  rep = trep;
	  count ++;
	}
	else {
	  rep = 0;
	  break;
	}
      }
    }
  } 
  else { // only one active
    rep = plotter ->getDataRep ( active );
    if ( rep ->hasNTupleBindings () == false ) {
      rep = 0;
    }
  }

  return rep;
}

void  DisplayController::setAxisBinding ( PlotterBase * plotter,
					  const std::string & axis,
					  const std::string & label )
{
  DataRep * rep = plotter -> getTarget ();

  if ( rep != 0 ) {
    rep -> setAxisBinding ( axis, label );
    Axes::Type a = Axes::X;
    if ( axis[0] == 'Y' ) a = Axes::Y;
    plotter -> setAutoRanging ( a, true );
    plotter -> autoScale ();
  }
}

void DisplayController::
setAxisBindings ( PlotterBase * plotter,
		  const std::vector< std::string > & labels ) const
{
  NTupleProjector * projector = getBindingProjector ( plotter );
  if ( projector == 0 ) return;

  projector->setAxisBindings ( labels );
  plotter->setAutoRanging ( true );

}

bool
DisplayController::
isDataValid ( const PlotterBase * plotter ) const
{
  bool valid = true;
  const DataRep * rep = plotter -> getTarget ();
  if ( rep != 0 ) {
    valid = isDataValid ( rep );
  }

  return valid;
}

bool
DisplayController::
isDataValid ( const DataRep * rep ) const
{
  bool valid = true;
  ProjectorBase * base = rep -> getProjector ();
  const NTupleProjector * projector 
    = dynamic_cast < const NTupleProjector * > ( base );
  if ( projector != 0 ) {
    valid = projector -> isDataValid ();
  }

  return valid;
}

bool
DisplayController::
getLog ( const PlotterBase * plotter, 
	 const std::string & axis) const
{
   Axes::Type at = Axes::convert ( axis );
   return getLog ( plotter, at );
}

/** @todo un-hard code this when we have XYZ transform? */
bool
DisplayController::
getLog ( const PlotterBase * plotter, hippodraw::Axes::Type axis ) const
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  XYTransform * xy_transform 
    = dynamic_cast < XYTransform *> ( plotter->getTransform () );

  if ( xy_transform == 0 ) return false;

  TransformBase * transform = 0;
 
 if ( axis == Axes::X ){
    transform = xy_transform->xTransform ();
  }
  if ( axis == Axes::Y ){
    transform = xy_transform->yTransform();
  }
  if ( axis == Axes::Z ){
    transform = xy_transform->zTransform();
  }

  assert ( transform );
  
  const string & name = transform->name ();

  return name == "Log";
}
void
DisplayController::
setBinner ( const PlotterBase * plotter,
	    DataRep * datarep, 
	    hippodraw::Axes::Type axis ) const
{
  ProjectorBase * projbase = datarep->getProjector ();

  BinningProjector * projector
    = dynamic_cast< BinningProjector * > ( projbase );
  if ( projector == 0 ) return;

  if ( axis == Axes::Y &&
       projector->isAxisBinned ( "Y" ) == false ) return;

  const Range & range = plotter -> getRange ( axis, false );
  BinnerAxis * binner = 0;

  BinnerAxisFactory * binner_factory = BinnerAxisFactory::instance ();
  AxisModelBase * model = plotter -> getAxisModel ( axis );

  if ( model->isLog() ) {
    binner = binner_factory -> create ( "BinnerLog" );
  } else { // not Log
    binner = binner_factory -> create ( "BinnerLinear" );
  }

  binner->setRange ( range, false );
  projector->setBinnerOn ( binner, axis );
}

/** @todo Remove this function when GUI has separate controls for
    displaying Log and binning log. 
*/
void
DisplayController::
setBinner ( PlotterBase * plotter, 
	    hippodraw::Axes::Type axis )
{
  int number = plotter->getNumDataReps ();
  for ( int i = 0; i < number; i++ ) {
    DataRep * datarep = plotter->getDataRep ( i );
    setBinner ( plotter, datarep, axis );
  }

}

void
DisplayController::
setAxisModel ( PlotterBase * plotter, 
	       hippodraw::Axes::Type axis,
	       const std::string & type )
{
  AxisModelBase * model = plotter->getAxisModel ( axis );

  if ( model != 0 ) {
    AxisModelBase * temp = 0;
  
    if ( type == "Linear" ) {
    
      if ( !( model->isLog() ) ) return; // no change needed
      temp = new AxisModelLinear ( *model );
  
    } 
    else if ( type == "Log" ) {
  
      if ( model->isLog () ) return; // no change needed
      const Range & range = plotter -> getRange ( axis, false );
      if ( range.high() < 0. ) {
	string what ( "Can not change axis to log scale since current "
		      "range is negative" );
	throw std::runtime_error ( what );
      }

      double pos = plotter->getPosRange ( axis );
      model->setRangePos ( pos );
      temp = new AxisModelLog ( *model );
  
    }
  
    assert ( temp );

    swap ( temp, model );

    plotter->setAxisModel ( model, axis );

    setBinner ( plotter, axis );

    // Do not delete until the end so project can use it.
    delete temp;
  }
}

void
DisplayController::
setLog ( PlotterBase * plotter, 
	 const std::string & axis, 
	 bool flag )
{
  Axes::Type at = Axes::convert ( axis );
  setLog ( plotter, at, flag );
}

/** @todo Check that display can do it.  */
void
DisplayController::
setLog ( PlotterBase * plotter,
	 hippodraw::Axes::Type axis,
	 bool flag )
{
  assert ( axis == Axes::X || axis == Axes::Y || axis == Axes::Z );

  if (dynamic_cast<TextPlotter *>(plotter) != NULL)
	return;
  
  TransformBase * current = plotter->getTransform ();

  XYTransform * xy_transform = dynamic_cast < XYTransform *> ( current );

  if ( xy_transform != 0 ) {
    setLog ( plotter, axis, flag, xy_transform );
    return;
  }
  assert ( axis == Axes::Z );
  BinaryTransform * b_transform 
    = dynamic_cast < BinaryTransform * > ( current );
  string type;
  if ( flag ) type = "Log";
  else type = "Linear";
  
  setAxisModel ( plotter, Axes::Z , type );

  TransformFactory * factory = TransformFactory::instance ();
  TransformBase * transform = factory -> createTransform ( type );

  b_transform -> setZTransform ( transform );
  plotter -> setTransform ( b_transform );
}

void
DisplayController::
setLog ( PlotterBase * plotter,
	 hippodraw::Axes::Type axis,
	 bool flag, 
	 XYTransform * xy_transform )
{
  TransformBase * transform = xy_transform->xTransform ();

  string x_trans = transform->name ();
  transform = xy_transform->yTransform ();
  string y_trans = transform->name ();
  transform = xy_transform->zTransform ();
  string z_trans = transform->name ();

  if( flag ){ //if the axis is to be made logarithmic
    if( axis == Axes::X ){
      x_trans = "Log";
      setAxisModel ( plotter, Axes::X, x_trans );
    }
    if( axis == Axes::Y ){
      y_trans = "Log";
      setAxisModel ( plotter, Axes::Y, y_trans );
    }
    if( axis == Axes::Z ){
      z_trans = "Log";
      setAxisModel ( plotter, Axes::Z, z_trans );
    }
  }
  
  if( !flag ){
    if( axis == Axes::X ) {
      x_trans = "Linear";
      setAxisModel ( plotter, Axes::X, x_trans );
    }
    if( axis == Axes::Y ) {
      y_trans = "Linear";
      setAxisModel ( plotter, Axes::Y , y_trans );
    }
    if( axis == Axes::Z ) {
      z_trans = "Linear";
      setAxisModel ( plotter, Axes::Z , z_trans );
    }
  }
  
  TransformFactory * factory = TransformFactory::instance ();

// The following verbosness for VC++ in debug mode.
  string t_name = x_trans;
  t_name += " ";
  t_name += y_trans;
  t_name += " ";
  t_name += z_trans;
  transform = factory->createTransform ( t_name );

  plotter->setTransform ( transform );
  delete transform; // The plotter made a copy and deleted the old one.
}


void DisplayController::setTransform ( PlotterBase * plotter,
				       const std::string & name )
{
  TransformFactory * factory = TransformFactory::instance ();
  TransformBase * transform = factory->createTransform ( name );

  try {
     plotter->setTransform ( transform );
     delete transform; // The plotter made a copy.
  } catch (PlotterException & eObj) {
     delete transform; // not used or needed
     throw eObj;
  }
}

void DisplayController::setTransform ( PlotterBase * plotter,
				       const std::string & x,
				       const std::string & y )
{
  TransformFactory * factory = TransformFactory::instance ();
  const string name = x + " " + y;
  TransformBase * transform = factory->createTransform ( name );

  plotter->setTransform ( transform );

  delete transform; // The plotter made a copy.
}

void DisplayController:: remove ( PlotterBase * plotter )
{
  delete plotter;
}

float DisplayController::pointSize ( const PlotterBase * plotter ) const
{
  RepBase * rep = plotter->representation ();
  
  return rep->size ();
}

bool
DisplayController::
hasSymbolRep ( const PlotterBase * plotter ) const
{
  RepBase * rep = plotter -> representation ();
  SymbolPointRep * symrep = dynamic_cast < SymbolPointRep * > ( rep );
  
  return symrep != 0;
}

bool
DisplayController::
hasLineRep ( const PlotterBase * plotter ) const
{
  bool yes = false;

  RepBase * rep = plotter -> representation ();
  ColumnPointRep * colrep = dynamic_cast < ColumnPointRep * > ( rep );
  
  if ( colrep != 0 ) {
    yes = true;
  }
  else {
    LinePointRep * linerep = dynamic_cast < LinePointRep * > ( rep );
    yes = linerep != 0;
  }

  return yes;
}

unsigned int
DisplayController::
getRepStyle ( const PlotterBase * plotter ) const
{
  RepBase * rep = plotter -> representation ();

  return rep -> getStyle ();
}

void DisplayController::setRepSize ( PlotterBase * plotter, float size )
{
  DataRep * datarep = plotter->selectedDataRep ();

  datarep->setRepSize ( size );
}

void
DisplayController::
setPointRep ( PlotterBase * plotter, const std::string & point_rep )
{
  int index = activeDataRepIndex ( plotter );
  if ( index >= 0 ) {
    DataRep * datarep = plotter -> getDataRep ( index );
    assert ( datarep );
    PointRepFactory * factory = PointRepFactory::instance ();
    RepBase * rep = factory -> create (  point_rep );
    datarep -> setPointRep ( rep );
  }
}

void
DisplayController::
setRange ( PlotterBase * plotter,
	   hippodraw::Axes::Type axis, 
	   const Range & range )
{
  plotter->setRange ( axis, range );

  plotter->setAutoRanging ( axis, false );
}

void 
DisplayController::
setBinWidth ( PlotterBase * plotter,
	      const std::string & axis,
	      double width )
{
  Axes::Type at = Axes::convert ( axis );

  setBinWidth ( plotter, at, width );
}

void
DisplayController::
setBinWidth ( DataRep * datarep,
	      hippodraw::Axes::Type axis,
	      double width )
{
  const list < Observer * > & obs_list = datarep -> getObservers ();
  list < Observer * >::const_iterator first = obs_list.begin();
  while ( first != obs_list.end () ) {
    Observer * obs = *first++;
    PlotterBase * plotter = dynamic_cast < PlotterBase * > ( obs );
    if ( plotter != 0 ) {
      plotter -> autoScale ();
      datarep -> setBinWidth ( axis, width );
      plotter -> setAutoRanging ( axis, false );
      break;
    }
  }
}

void
DisplayController::
setBinWidth ( PlotterBase * plotter,
	      hippodraw::Axes::Type axis,
	      double width )
{
  plotter -> setBinWidth ( axis, width );
  plotter->setAutoRanging ( axis, false );
}

/** @todo Use Axes::convert. 

    @todo Remove duplicated code compared to DisplayController::setBinWidth. 
*/
void
DisplayController::
setOffset ( PlotterBase * plotter,
	    const std::string & axis,
	    int parm,
	    bool dragging )
{
  bool widths_are_same = false;

  Axes::Type at = Axes::X;
  if ( axis == "Y" ) at = Axes::Y;
  if ( axis == "Z" ) at = Axes::Z;
  plotter->setAutoRanging ( at, false );

  int index = plotter->activePlotIndex ();

  if ( index < 0 ) {
    int number = plotter->getNumDataReps ();
    for ( int i = 0; i < number; i++ ) {
      ProjectorBase * projector = plotter->getProjector ( i );
      projector->setOffset ( axis, parm, dragging );
    }

    widths_are_same = true;
  }
  else {
    ProjectorBase * projector = plotter->getProjector ( index );
    projector->setOffset ( axis, parm, dragging );
  }

}

void
DisplayController::
setOffset ( PlotterBase * plotter,
	    hippodraw::Axes::Type axis,
	    double offset )
{
  bool widths_are_same = false;
  
  int index = plotter->activePlotIndex ();
  
  if ( index < 0 ) {
    int number = plotter->getNumDataReps ();
    for ( int i = 0; i < number; i++ ) {
      ProjectorBase * projector = plotter->getProjector ( i );
      projector->setOffset ( axis, offset );
    }

    widths_are_same = true;
  }
  else {
    ProjectorBase * projector = plotter->getProjector ( index );
    projector->setOffset ( axis, offset );
  }

}

void
DisplayController::
setErrorDisplayed ( const PlotterBase * plotter, 
		    hippodraw::Axes::Type axis,
		    bool state ) const
{
  int index = activeDataRepIndex ( plotter );
  if ( index >= 0 ) {
    DataRep * rep = plotter -> getDataRep ( index );
    rep -> setErrorDisplay ( axis, state );
  }
}

void
DisplayController::
createNTuple ( const PlotterBase * plotter )
{
  int index = plotter -> activePlotIndex ();
//   if ( index < 0 ) return;

  NTuple * ntuple = plotter -> createNTuple ();
  DataSourceController::instance () -> registerNTuple ( ntuple );
}

void
DisplayController::
setTransformAxis ( PlotterBase * plotter, 
                   const std::string & x, 
                   const std::string & y)
{
  setAxisModelWithoutSetBin ( plotter, Axes::X, x );
  setAxisModelWithoutSetBin ( plotter, Axes::Y, y );
}


void
DisplayController::
setAxisModelWithoutSetBin ( PlotterBase * plotter, 
	                    hippodraw::Axes::Type axis,
	                    const std::string & type )
{
  AxisModelBase * model = plotter->getAxisModel ( axis );

  if ( model != 0 ) {
    AxisModelBase * temp = 0;
  
    if ( type == "Linear" ) {
    
      if ( !( model->isLog() ) ) return; // no change needed
      temp = new AxisModelLinear ( *model );
  
    } 
    else if ( type == "Log" ) {
  
      if ( model->isLog () ) return; // no change needed    
      const Range & range = plotter -> getRange ( axis, false );
      if ( range.high () < 0. ) {
	string what ( "Can not change axis to Log scale since current "
		      "range is negative." );
	throw std::runtime_error ( what );
      }
      double pos = plotter->getPosRange ( axis );
      model->setRangePos ( pos );
      temp = new AxisModelLog ( *model );
  
    }
  
    assert ( temp );

    swap ( temp, model );

    plotter->setAxisModel ( model, axis );

    // Do not delete until the end so project can use it.
    delete temp;
  }
}
