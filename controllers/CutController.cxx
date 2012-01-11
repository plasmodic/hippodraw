/** @file

hippodraw::CutController implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CutController.cxx,v 1.167 2007/06/07 18:16:05 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "CutController.h"

#include "DisplayController.h"
#include "DataRepController.h"

#include "datareps/DataRep.h"
#include "datareps/DataRepFactory.h"
#include "datasrcs/DataSource.h"
#include "datasrcs/TupleCut.h"

#include "graphics/ViewBase.h"

#include "plotters/PlotterBase.h"
#include "plotters/Cut1DPlotter.h"
#include "plotters/Cut2DPlotter.h"
#include "plotters/PlotterFactory.h"

#include "projectors/NTupleProjector.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <cassert>

using std::find;
using std::list;
using std::map;
using std::string;
using std::vector;
using std::make_pair;
using std::pair;
using std::logic_error;
using std::runtime_error;

using namespace hippodraw;

CutController * CutController::s_instance = 0;

CutController::CutController ( )
{
}

CutController * CutController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new CutController ( );
  }
  return s_instance;
}

Cut1DPlotter *
CutController::
addCut ( PlotterBase * plotter, const std::string & label )
{
  vector < string > bindings;
  bindings.push_back ( label );
  PlotterBase * cut = addCut ( plotter, bindings );

  return dynamic_cast < Cut1DPlotter * > ( cut );
}

PlotterBase *
CutController::
addCut( PlotterBase * plotter,	std::vector < std::string > & bindings )
{
  DisplayController * controller = DisplayController::instance ();
  const DataSource * source = controller->getDataSource ( plotter, 
      controller->activeDataRepIndex(plotter) );
  const Color color ( Color::yellow );
  CutPlotter * cut_plotter = createCut ( "", source, bindings, color );

  addCut ( cut_plotter, plotter );
  
  return cut_plotter;
}


void
CutController::
addCut ( PlotterBase * src_plotter, PlotterBase * plotter )
{
  CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( src_plotter );
  if ( cut_plotter == 0 ) { // not a cut
    string what ( "Argument was not a cut" );
    throw runtime_error ( what );
  }

  int index = plotter->activePlotIndex ();
  if ( index < 0 ) { // multiple reps are active
    unsigned int size = plotter -> getNumDataReps ();
    for ( unsigned int i = 0; i < size; i++ ) {
      DataRep * targetrep = plotter -> getDataRep ( i );
      if ( targetrep -> hasNTupleBindings () ) {
	linkCutAndRep ( cut_plotter, targetrep );
      }
    }
  }
  else { // single data rep is active
    DataRep * targetrep = plotter->getDataRep ( index );
    if ( targetrep -> hasNTupleBindings() ) {
      linkCutAndRep( cut_plotter, targetrep);
    }
  }

  CutPlotter * cp = dynamic_cast < CutPlotter * > ( plotter );
  if ( cp != 0 ) {
    const list < DataRep * > & targets = cp -> getCutTargets ();
    list < DataRep * > :: const_iterator first = targets.begin();
    while ( first != targets.end () ) {
      DataRep * target = *first++;
      linkCutAndRep ( cut_plotter, target );
    }
  }
}

void
CutController::
addCuts ( const std::vector < PlotterBase * > & cut_list,
	  PlotterBase * plotter )
{
  unsigned int size = cut_list.size ();

  for ( unsigned int i = 0; i < size; i++ ) {
    PlotterBase * pb = cut_list[i];
    CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( pb );
    if ( cut_plotter != plotter ) {
      addCut ( cut_plotter, plotter );
    }
  }
}

void
CutController::
linkCutAndRep( CutPlotter * cut_plotter,
	       DataRep * targetrep )
{
  const vector < TupleCut > & cuts = cut_plotter -> getCuts ();
  assert ( cuts.empty () == false );

  ProjectorBase * projbase = targetrep->getProjector ();
  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( projbase );
  assert ( projector );

  for ( unsigned int i = 0; i < cuts.size(); i++ ) {
    projector->addCut ( &cuts[i] );
  }
  
  cut_plotter->addCutTarget ( targetrep );
  targetrep->setDirty ();
}

CutPlotter * 
CutController::
createCut ( const std::string & ,
	    const DataSource * ntuple,
	    const std::vector < std::string > & bindings,
	    const Color & color ) const
{
  std::string datarepname, plottername;
  int ndims =  bindings.size();
  
  if( ndims == 1 )
    {
      datarepname = "Histogram";     // 1 D DyHistogram (Dynamic Histogram)
      plottername = "Cut1DPlotter";  // 1 D cut plotter
    }
  else if( ndims == 2 )
    {
      datarepname = "Color Plot";    // 2 D couterpart of DyHistogram
      plottername = "Cut2DPlotter";  // 2 D cut plotter
    }
  
  DataRepController * controller = DataRepController::instance ();
  DataRep * rep = controller->createDataRep ( datarepname,
					      ntuple,
					      bindings );
  assert( rep != 0 );
  
  PlotterFactory * pfactory = PlotterFactory::instance ();
  PlotterBase * pbase = pfactory->create ( plottername );
  CutPlotter * plotter = dynamic_cast < CutPlotter * > ( pbase );

  // Add data rep and tuple cut separately.
  plotter -> addDataRep ( rep );
  plotter -> addTupleCut ( rep );

  plotter -> setNTuple ( ntuple );
  plotter -> setAxisBinding ( bindings );
  plotter -> setCutRangeFull ();
  plotter -> setCutColor ( color );

  return plotter;
}

void CutController::removeCut ( PlotterBase * cplotter, 
				PlotterBase * plotter )
{
  CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( cplotter );

  if ( cut_plotter != 0 ) {
    DataRep * rep = plotter -> selectedDataRep (); 

    if ( rep != 0 ) {
      cut_plotter -> removeFromTarget ( rep );
      setZoomPan ( cut_plotter, Axes::X, false );
    }
  }
  else { // must be cut for region
    DataRep * rep = cplotter -> selectedDataRep ();
    rep -> removeCut ();
  }
}

void
CutController::
fillCutList ( const std::vector < PlotterBase * > & plotter_list,
	      std::vector < CutPlotter * > & cut_list )
{
  cut_list.clear ();
  vector < PlotterBase * >::const_iterator first = plotter_list.begin();
  while ( first != plotter_list.end() ) {
    PlotterBase * plotter = *first++;
    CutPlotter * cutter = dynamic_cast < CutPlotter * > ( plotter );
    if ( cutter != 0 ) {
      cut_list.push_back ( cutter );
    }
  }
}

const vector < const TupleCut * > &
CutController::
getCutList ( const DataRep * datarep ) const
{
  ProjectorBase * pbase = datarep -> getProjector ();
  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( pbase );

  if ( projector == 0 ) { // then can't have cuts
    string what ( "CutController::getCutList: ");
    what += "DataRep does not have NTupleProjector.";
    throw std::logic_error ( what );
  }

  return projector -> getCutList();
}

void
CutController::
fillCutList ( const PlotterBase * plotter,
	      std::vector < PlotterBase * > & cut_list )
{
  cut_list.clear();
  int reps = plotter -> getNumDataReps ();
  for ( int i = 0; i < reps; i++ ) {
    const DataRep * rep = plotter -> getDataRep ( i );
    vector < PlotterBase * > cuts;
    fillCutList ( rep, cuts );
    cut_list.insert ( cut_list.end(), cuts.begin(), cuts.end() );
  }
}

void
CutController::
fillCutList ( const DataRep * datarep,
	      std::vector < PlotterBase * > & cut_list )
{
  cut_list.clear ();

  const Observable::ObserverList_t & objects = datarep -> getObservers ();
  Observable::ObserverList_t::const_iterator first = objects.begin();

  while ( first != objects.end() ) {
    const Observer * obj = *first++;
    const CutPlotter * plotter = dynamic_cast < const CutPlotter * > ( obj );
    if ( plotter != 0 ) {
      const DataRep * rep = plotter -> getDataRep ( 0 );
      if ( rep != datarep ) {
	CutPlotter * cp = const_cast < CutPlotter * > ( plotter );
	cut_list.push_back ( cp );
      }
    }
  }
}

void
CutController::
fillCutWeb ( const std::vector < PlotterBase * > & plotters,
	     std::vector < PlotterBase * > & web )
{
  web.clear ();
  vector < PlotterBase * > ::const_iterator first = plotters.begin ();

  while ( first != plotters.end () ) {
    PlotterBase * plotter = *first++;
    appendToWeb ( plotter, web );
  }
}


void
CutController::
appendToWeb ( PlotterBase * plotter, PlotterList_t & web )
{
  PlotterList_t::iterator first = find ( web.begin (), web.end(),
					 plotter );
  if ( first == web.end () ) {
    web.push_back ( plotter );
    int index = plotter -> activePlotIndex ();
    if ( index < 0 ) {
      web.clear ();
      return;
    }
    DataRep * rep = plotter -> getDataRep ( index );
    vector < PlotterBase * > cut_list;
    fillCutList ( rep, cut_list );
    if ( cut_list.empty () == false ) {
      appendToWeb ( cut_list, web );
    }
  }
}

void
CutController::
appendToWeb ( CutPlotter * cutter,
	      PlotterList_t & web )
{
  PlotterList_t::iterator first = find ( web.begin(), web.end(),
					 cutter );
  if ( first == web.end () ) {
    web.push_back ( cutter );
    // get the targets and make recurive call
    const list < DataRep * > & targets = cutter -> getCutTargets ();
    list < DataRep * > ::const_iterator it = targets.begin ();

    while ( it != targets.end () ) {
      DataRep * rep = *it++;
      PlotterBase * plotter = findPlotter ( rep );
      assert ( plotter );
      appendToWeb ( plotter, web );
    }
  }
}

void
CutController::
appendToWeb ( const std::vector < PlotterBase * > & cutters,
	      PlotterList_t & web )
{
  PlotterList_t::const_iterator first = cutters.begin ();
  while ( first != cutters.end () ) {
    PlotterBase * pb = *first++;
    CutPlotter * cutter = dynamic_cast < CutPlotter * > ( pb );
    appendToWeb ( cutter, web );
  }
}

PlotterBase *
CutController::
findPlotter ( const DataRep * datarep )
{
  const PlotterBase * plotter = 0;
  const Observable::ObserverList_t & objects = datarep -> getObservers ();
  Observable::ObserverList_t::const_iterator first = objects.begin();

  while ( first != objects.end () ) {
    const Observer * obj = *first++;
    const CutPlotter * cutter = dynamic_cast < const CutPlotter * > ( obj );
    if ( cutter != 0 ) {
      DataRep * plotter_rep = cutter -> getDataRep ( 0 );
      if ( plotter_rep == datarep ) {
	plotter = cutter;
	break;
      }
      continue; 
    }
    const XyPlotter * xyplotter = dynamic_cast < const XyPlotter * > ( obj );
    if ( xyplotter != 0 ) {
      plotter = xyplotter;
      break;
    }
  }

  return const_cast < PlotterBase * > ( plotter );
}

void 
CutController::
fillTupleCutList ( const std::vector < const ViewBase * > & views,
		   std::vector < const TupleCut * > & cut_list )
{
  cut_list.clear();

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  vector < const ViewBase * >::const_iterator first = views.begin ();
  while ( first != views.end() ) {
    const ViewBase * view = *first++;
    PlotterBase * pbase = view->getPlotter ();
    Cut1DPlotter * plotter = dynamic_cast < Cut1DPlotter * > ( pbase );
    if ( plotter == 0 ) continue;

    const vector < TupleCut > & cuts = plotter -> getCuts ();
    for ( unsigned int i = 0; i < cuts.size(); i++ ) {
      cut_list.push_back ( &cuts[i] );
    }
  }
}

void 
CutController::
connectDataRep ( const std::list < ViewBase * > & targets,
		 const std::vector < const ViewBase * > & views )
{
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < ViewBase * > :: const_iterator first = targets.begin ();
  while( first != targets.end () )
    {
      ViewBase * view = *first++;
      PlotterBase * plotter = view->getPlotter ();
      int number = plotter -> getNumDataReps ();

      for( int i = 0; i < number; i++ )
	{
	  DataRep * rep = plotter->getDataRep ( i );
	  
	  if ( rep->hasNTupleBindings () == false )
	    continue;
	  
	  connectDataRep ( rep, views );
	}
    }
}

void 
CutController::
connectDataRep( DataRep * rep, 
		const std::vector < const ViewBase * > & views )
{
  ProjectorBase * pbase = rep->getProjector ();
  NTupleProjector * projector 
    = dynamic_cast < NTupleProjector * > ( pbase );

  if ( projector == 0 ) return; // no cuts possible.

  const vector < const TupleCut * > & cuts = projector->getCutList ();
  if ( cuts.empty() == true ) return;

#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
    vector < const TupleCut * > ::const_iterator first = cuts.begin ();
  while ( first != cuts.end() ) {
    const TupleCut * cut = *first++;
    connectDataRep ( cut, rep, views );
  }
}

void 
CutController::
connectDataRep ( const TupleCut * cut, 
		 DataRep * rep,
		 const std::vector < const ViewBase * > & views )
{
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
    vector < const ViewBase * >::const_iterator first = views.begin();
  while ( first != views.end() )
    {
      const ViewBase * view = *first++;
      PlotterBase * pbase = view->getPlotter ();
      
      Cut1DPlotter * plotter = dynamic_cast < Cut1DPlotter * > ( pbase );
      if ( plotter == 0 ) continue;
      
      const vector < TupleCut > & cuts = plotter -> getCuts ();
      for ( unsigned int i = 0; i < cuts.size(); i++ ) {
	if ( &cuts[i] == cut ) {
	  plotter->addCutTarget ( rep );
	}
      }
    }
}

const vector < PlotterBase * > &
CutController::
getCutList ( const std::vector < PlotterBase * > & plotters,
	     const DataSource * ntuple ) const
{
  DisplayController * dc = DisplayController::instance ();

  m_cut_list.clear ();
  std::vector < PlotterBase * >::const_iterator it = plotters.begin();

  for ( ; it != plotters.end(); ++it )
    {
      PlotterBase * plotter = *it;
      CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( plotter );

      if ( cut_plotter != 0 ) {
	const DataSource * cut_tuple = dc ->getDataSource ( cut_plotter, 0 );
	if ( cut_tuple == ntuple ) {
	  m_cut_list.push_back ( cut_plotter );
	}
      }
    }
  
  return m_cut_list;
}

void
CutController::
setZoomPan ( PlotterBase * cut_plotter, Axes::Type axis, bool yes )
{
  if ( yes ) 
    m_zoom_pan.push_back ( make_pair( cut_plotter, axis ) );
  else  // no
    m_zoom_pan.remove ( make_pair( cut_plotter, axis ) );
}

bool
CutController::
isZoomPan ( const PlotterBase * cut_plotter,
	    Axes::Type axis ) const
{
  bool found = false;
  
  list< pair< PlotterBase *, Axes::Type > >::const_iterator iter; 
    
  for( iter = m_zoom_pan.begin(); iter != m_zoom_pan.end(); iter++ )
    if( iter -> first  == cut_plotter && iter -> second == axis )
      found = true;
  
  return found;
}

void
CutController::
fillAcceptedRows ( std::vector < bool > & acceptArray,
		   const DataSource * source,
		   const std::vector < const TupleCut * > & cut_list )
{
  acceptArray.clear ();

  std::size_t size = source -> rows ();
  acceptArray.reserve ( size );

  std::size_t num_cuts = cut_list.size ();
  for ( unsigned int i = 0; i < size; i++ )
    {
      // If cut is not selected, default is accept.
      bool accept = true;

      // Check all the cuts.
      for ( unsigned int j = 0; j < num_cuts; j++ ) 
	{
	  const TupleCut * tc = cut_list[j];
	  accept = tc -> acceptRow ( source, i );
	  if (!accept) break;
	}

      acceptArray.push_back ( accept );
    }
}

template < typename T >
class dequal {
  T val;
public:
  dequal ( const T &x ){ val = x; }
  bool operator == ( const T & y ) const { return val - y < 1.0;}
};

bool operator == ( const double & x,
		   const dequal<double> & d )
{
  return d == x;
}

void
CutController::
createIdCut ( const DataSource * source, DataSource * dest )
{
  const string colname = source -> getLabelAt( 0 );
  const vector < double > & destcol = dest -> getColumn ( colname );
  const vector < double > & srccol = source -> getColumn ( 0 );
  const string colname_cut = colname + "_cut";
  std::size_t dest_size = dest -> rows ();
  vector < double > dest_cut ( dest_size, 0. );
  for ( std::size_t i = 0; i < srccol.size (); i++ ) {
    const double & value = srccol[i];
    vector < double > ::const_iterator first 
      = find ( destcol.begin(), destcol.end(),
	       dequal< double> ( value ) );
    std::size_t d = distance ( destcol.begin(), first );
    dest_cut [d] = 1.0;
  }
  dest -> addColumn ( colname_cut, dest_cut );
}
