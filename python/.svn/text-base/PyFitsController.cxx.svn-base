/** @file

hippodraw::PyFitsController implementation.

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyFitsController.cxx,v 1.18 2007/07/02 18:17:15 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PyFitsController.h"

#include "PyApp.h"
#include "PyDataSource.h"
#include "QtCut.h"

#include "fits/FitsController.h"
#include "fits/FitsNTuple.h"

#include <stdexcept>

using std::runtime_error;
using std::vector;

using namespace hippodraw;

FitsController   * PyFitsController::m_instance = 0;
PyFitsController * PyFitsController::s_instance = 0;


PyFitsController::
PyFitsController()
{
}

/** @request Should always build this class and throw exception if
    user tries to use it when code was not built with FITS support.
 */
PyFitsController *
PyFitsController::
instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new PyFitsController ();
    m_instance = FitsController::instance ();
  }

  return s_instance;
}

FitsNTuple *
PyFitsController::
createNTuple ( const std::string & filename, const std::string & hduname )
{
  FitsNTuple * ftuple = 0;

  try {
    DataSource * ntuple = m_instance -> createNTuple ( filename, hduname );
    ftuple = dynamic_cast < FitsNTuple * > ( ntuple );
  }
  catch ( const runtime_error & e ) {
    throw e;
  }
  return ftuple;
}

PyDataSource *
PyFitsController::
createDataArray ( const std::string & filename, const std::string & hduname )
{
#ifdef HAVE_NUMARRAY
  FitsNTuple * tuple = createNTuple ( filename, hduname );
  PyDataSource * ds = new PyDataSource ( "FitsNTuple", tuple );
  return ds;
#else
   runtime_error e ( "HippoDraw was not built with numeric Python support" );
   throw e;
#endif
}

void
PyFitsController::
writeToFile ( const DataSource * source, const std::string & filename )
{
  // Need lock because on multi-cpu machine, GUI thread maybe trying
  // to display something based on this source.
  PyApp::lock();
  m_instance -> writeNTupleToFile ( source, filename );
  PyApp::unlock ();
}

void
PyFitsController::
writeToFile ( const PyDataSource * source, const std::string & filename )
{
  const DataSource & data = source  -> dataSource ();
  writeToFile ( & data, filename );
}

void
PyFitsController::
writeToFile ( const DataSource * source,
	      const std::string & filename,
	      const std::vector < QtCut * > & cut_list,
	      const std::vector < std::string > & column_list )
{
  // Need lock because on multi-cpu machine, GUI thread maybe trying
  // to display something based on this source.
  PyApp::lock ();
  vector < const TupleCut * > tuple_cut_list;
  QtCut::fillCutList ( tuple_cut_list, cut_list );
  
  // Need to do something with retval.
  // int retval =
  const std::string & title = source -> title ();

  m_instance -> writeNTupleToFile ( source, filename, title,
				    column_list, tuple_cut_list );
  PyApp::unlock ();
}

void
PyFitsController::
writeToFile ( const PyDataSource * array,
	      const std::string & filename,
	      const std::vector < QtCut * > & cut_list,
	      const std::vector < std::string > & column_list )
{
  const DataSource & source = array -> dataSource ();
  writeToFile ( & source, filename, cut_list, column_list );
}

const std::vector < std::string > &
PyFitsController::
getNTupleNames ( const std::string & filename )
{
  try {
    const std::vector< std::string > & names 
      =  m_instance -> getNTupleNames ( filename );
    return names;
  }
  catch ( const runtime_error & e ) {
    throw e;
  }
  static const std::vector < std::string > names;

  return names;
}
