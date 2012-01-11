/** @file

hippodraw::PyNTupleController implementation.

Copyright (C) 2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: PyNTupleController.cxx,v 1.11 2007/03/27 18:53:05 pfkeb Exp $

*/

#ifdef _MSC_VER
// nonstandard extension used 'extern' before...
# pragma warning(disable:4231)  

// needs to have dll-interface used by client
# pragma warning(disable:4251)  

// non dll-interface struct
# pragma warning(disable:4275)  

// 'int' : forcing value to bool 'true' or 'false' (performance warning)
# pragma warning(disable:4800)  
#endif

#include "PyNTupleController.h"

#include "PyApp.h"
#include "PyDataSource.h"
#include "PyNTuple.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTupleController.h"

#include <stdexcept>

using namespace hippodraw;

NTupleController   * PyNTupleController::m_instance = 0;
PyNTupleController * PyNTupleController::s_instance = 0;


PyNTupleController::
PyNTupleController()
{
}

PyNTupleController *
PyNTupleController::
instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new PyNTupleController ();
    m_instance = NTupleController::instance ();
  }

  return s_instance;
}

NTuple *
PyNTupleController::
createNTuple ( )
{
  PyNTuple * ntuple = new PyNTuple ();
  DataSourceController * controller = DataSourceController::instance ();
  controller -> registerNTuple ( ntuple );

  return ntuple;
}

DataSource *
PyNTupleController::
createNTuple ( const std::string & filename )
{
  PyApp::lock ();
  unsigned int columns = 0;
  PyNTuple * ntuple = new PyNTuple ( columns );

  try {
    m_instance -> fillFromFile ( filename, ntuple );
  }
  catch ( const std::runtime_error & e ) {
    PyApp::unlock ();
    delete ntuple;
    throw e;
  }
  PyApp::unlock ();

  return ntuple;
}

CircularBuffer *
PyNTupleController::
createCircularBuffer ( unsigned int columns )
{
  return m_instance -> createCircularBuffer ( columns );
}

PyDataSource *
PyNTupleController::
createDataArray ( const std::string & filename )
{
#ifdef HAVE_NUMARRAY
  DataSource * tuple = createNTuple ( filename );
  PyDataSource * ds = new PyDataSource ( "NTuple", tuple );

  return ds;
#else
  std::runtime_error e ( "HippoDraw was not built with "
			 "numeric Python support" );
   throw e;
#endif
}

DataSource *
PyNTupleController::
findDataSource ( const std::string & name ) const
{
  DataSourceController * dsc = DataSourceController::instance ();

  return dsc -> findDataSource ( name );
}

std::string
PyNTupleController::
registerNTuple ( DataSource * source )
{
  PyApp::lock ();
  DataSourceController * dsc = DataSourceController::instance ();

  std::string name = dsc -> registerNTuple ( source );
  PyApp::unlock ();

  return name;
}

void
PyNTupleController::
registerNTuple ( const std::string & name, DataSource * source )
{
  DataSourceController * dsc = DataSourceController::instance ();

  dsc -> registerNTuple ( name, source );
}

int
PyNTupleController::
writeNTupleToFile ( DataSource * source,
		    const std::string & filename )
{
  return m_instance -> writeNTupleToFile ( source, filename );
}

void
PyNTupleController::
changeName ( DataSource * source, const std::string & new_name )
{
  source -> setName ( new_name );
}
