/** @file

hippodraw::QtCut class implementation

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtCut.cxx,v 1.66 2007/07/02 18:17:15 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "QtCut.h"

#include "PyApp.h"
#include "PyDataSource.h"

#include "controllers/CutController.h"
#include "datasrcs/TupleCut.h"
#include "datasrcs/NTupleController.h"
#include "fits/FitsController.h"
#include "plotters/CutPlotter.h"

using std::string;
using std::vector;

using namespace hippodraw;

void
QtCut::
createCut ( const DataSource & nt,
	    const std::vector < std::string > & binding )
{
  CutController * controller = CutController::instance ();
  const Color yellow ( Color::yellow );
  m_plotter = controller -> createCut ( std::string(), &nt, binding, yellow );
}

QtCut::
QtCut ( const DataSource & nt, 
	const std::vector< std::string > & binding,
	QtDisplay * target, double low, double high  )
   : QtDisplay ( )
{
  PyApp::lock ();
  createCut ( nt, binding );

  addTarget ( target );
  setCutRange ( low, high, "x" );
  PyApp::unlock ();
}

QtCut::
QtCut ( const PyDataSource & ds, 
	const std::vector< std::string > & binding,
	QtDisplay * target, double low, double high  )
   : QtDisplay ( )
{
  PyApp::lock ();
  createCut ( ds.dataSource(), binding );

  addTarget ( target );
  setCutRange ( low, high, "x" );
  PyApp::unlock ();
}

QtCut::
QtCut ( PyDataSource & source,
	const std::string & label,
	boost::python::numeric::array array,
	QtDisplay * target )
{
#ifdef HAVE_NUMARRAY
  PyApp::lock ();

  source.saveColumnFromNumArray ( label, array );
  vector < string > binding;
  binding.push_back ( label );
  createCut ( source.dataSource (), binding );
  addTarget ( target );
  setCutRange ( 0.9, 1.1, "x" );

  PyApp::unlock ();
#else
  std::runtime_error e ( "HippoDraw was not built with "
		       "numeric Python support" );
  throw e;

#endif // HAVE_NUMARRAY
}

QtCut::
QtCut ( const DataSource & nt, 
	const std::vector< std::string > & binding )
   : QtDisplay ( )
{
  PyApp::lock ();
  createCut ( nt, binding );
  PyApp::unlock ();
}

QtCut::
QtCut ( const PyDataSource & nt, 
	const std::vector< std::string > & binding )
   : QtDisplay ( )
{
  PyApp::lock ();
  createCut ( nt.dataSource (), binding );
  PyApp::unlock ();
}

QtCut::QtCut ( PlotterBase * plotter ) 
   : QtDisplay()
{
  PyApp::lock ();
  CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( plotter );
  assert ( cut_plotter != 0 );

  m_plotter = plotter;
  PyApp::unlock ();
}

void
QtCut::
addTarget ( QtDisplay * target )
{
  PyApp::lock();

  CutController * controller = CutController::instance ();
  CutPlotter * cp = dynamic_cast < CutPlotter * > ( m_plotter );
  controller -> addCut ( cp, target -> display () );
  
  PyApp::unlock ();
}

void QtCut::addTargets ( const std::vector < QtDisplay * > & targets )
{
  PyApp::lock();

  CutController * controller = CutController::instance ();
  unsigned int size = targets.size ();
  CutPlotter * cp = dynamic_cast < CutPlotter * > ( m_plotter );
  for ( unsigned int i = 0; i < size; i++ ) {
    QtDisplay * target = targets [ i ];
    controller -> addCut ( cp, target -> display () );
  }
  
  PyApp::unlock ();
}

void
QtCut::
setCutRange ( double low, double high, const std::string & axis )
{
  hippodraw::Axes::Type type = hippodraw::Axes::convert( axis );

  PyApp::lock();
  
  CutPlotter * cut_plotter = dynamic_cast < CutPlotter * > ( m_plotter );
  assert ( cut_plotter != 0 );

  const Range range ( low, high );
  try {
    cut_plotter -> setCutRangeAt ( range, type );    
  PyApp::unlock ();
  }
  catch ( const std::out_of_range &  ) { // if axis is wrong
    PyApp::unlock ();
    string what ( "Cut range axis `" );
    what += axis;
    what += "' is invalid";
    throw std::runtime_error ( what );
  }
}


std::vector<double> QtCut::cutRange ()
{
  PyApp::lock();

  std::vector<double> myCutRange;
  vector < const TupleCut * > cuts;
  m_plotter -> fillCutList ( cuts );

  for ( unsigned int i = 0; i < cuts.size(); i++ ) {
    const TupleCut * cut = cuts [ i ];
    const Range & range = cut -> getRange ();
    myCutRange.push_back ( range.low () );
    myCutRange.push_back ( range.high () );
  }
  PyApp::unlock ();

  return myCutRange;
}

void
QtCut::
setEnabled ( bool yes )
{
  PyApp::lock();
  CutPlotter * plotter = dynamic_cast < CutPlotter * > ( m_plotter );
  plotter -> setEnabled ( yes );
  PyApp::unlock ();
}

void
QtCut::
toggleInverted ()
{
  PyApp::lock();
  CutPlotter * plotter = dynamic_cast < CutPlotter * > ( m_plotter );
  plotter -> toggleInverted ();
  PyApp::unlock ();
}

NTuple *
QtCut::
createNTuple ( const std::vector < std::string > & column_list,
	       const std::vector < QtCut * > & cut_list, 
	       DataSource * ds )
{

  PyApp::lock();

  std::vector < const TupleCut * > tuple_cut_list;
  std::vector < QtCut * >::const_iterator it = cut_list.begin();
  
  // QtCut list to TupleCut list.
  for ( ; it !=cut_list.end(); ++it )
    {
      QtCut * cut = *it;
      cut->m_plotter-> fillCutList ( tuple_cut_list );
    }

  NTupleController * ntc = NTupleController::instance();
  NTuple * nt = ntc->createNTuple ( column_list, tuple_cut_list, ds );

  PyApp::unlock();

  return nt;

}

void
QtCut::
createTnt ( const std::vector < std::string > & column_list,
	    const std::vector < QtCut * > & cut_list, 
	    DataSource * ds,
	    const std::string & filename,
	    const std::string & dsname )
{

  PyApp::lock();

  std::vector < const TupleCut * > tuple_cut_list;
  std::vector < QtCut * >::const_iterator it = cut_list.begin();
  
  // QtCut list to TupleCut list.
  for ( ; it !=cut_list.end(); ++it )
    {
      QtCut * cut = *it;
      cut->m_plotter-> fillCutList ( tuple_cut_list );
    }

  NTupleController * ntc = NTupleController::instance();
  
  // Need to do something with retval.
  // int retval =
  ntc->createNTupleToFile(column_list, tuple_cut_list, ds, filename, dsname);

  PyApp::unlock();

}

void
QtCut::
createFits ( const std::vector < std::string > & column_list,
	    const std::vector < QtCut * > & cut_list, 
	    DataSource * ds,
	    const std::string & filename,
	    const std::string & dsname )
{
#ifdef HAVE_CFITSIO
  PyApp::lock();

  std::vector < const TupleCut * > tuple_cut_list;
  std::vector < QtCut * >::const_iterator it = cut_list.begin();
  
  // QtCut list to TupleCut list.
  for ( ; it !=cut_list.end(); ++it )
    {
      QtCut * cut = *it;
      cut->m_plotter-> fillCutList ( tuple_cut_list );
    }

  FitsController * fc = FitsController::instance();
  
  // Need to do something with retval.
  // int retval =
  fc -> writeNTupleToFile(ds, filename, dsname, column_list, tuple_cut_list );

  PyApp::unlock();
#else
  std::string what ( "Sorry, can not create FITS file.  hippo module was not "
		"built with optional FITS support" );
  throw std::runtime_error ( what );
#endif

}

void
QtCut::
fillCutList ( std::vector <  const TupleCut *> & tuple_cut_list,
	      const std::vector < QtCut * > & cut_list )
{
  std::vector < QtCut * >::const_iterator it = cut_list.begin();
  for ( ; it !=cut_list.end(); ++it )
    {
      QtCut * cut = *it;
      cut -> m_plotter-> fillCutList ( tuple_cut_list );
    }
}
