/** @file

hippodraw::QtRootController class implementatoin.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtRootController.cxx,v 1.27 2007/07/02 18:17:16 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "QtRootController.h"

#include "RootController.h"
#include "QtRootNTuple.h"

#include "datasrcs/DataSourceController.h"

#include "python/PyApp.h"
#include "python/PyDataSource.h"

using namespace hippodraw;

RootController   * QtRootController::m_instance = 0;
QtRootController * QtRootController::s_instance = 0;

QtRootController::
QtRootController()
{
}

QtRootController *
QtRootController::
instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new QtRootController ();
    m_instance = RootController::instance ();
  }

  return s_instance;
}

DataSource *
QtRootController::
createNTuple ( const std::string & filename, const std::string & treename )
{
  PyApp::lock();

  TTree * tree = m_instance -> getTree ( filename, treename );
  QtRootNTuple * rtuple = new QtRootNTuple ( tree );
  DataSource * ntuple = rtuple;

  PyApp::unlock ();

  return m_instance -> initNTuple ( ntuple, filename, treename );
}

PyDataSource *
QtRootController::
createDataArray ( const std::string & filename, const std::string & treename )
{
#ifdef HAVE_NUMARRAY
  DataSource * rtuple = createNTuple ( filename, treename );
  PyDataSource * ds = new PyDataSource ( "RootNTuple", rtuple );

  return ds;
#else
  std::runtime_error e ( "HippoDraw was not built with "
			 "numeric Python support" );
   throw e;
#endif
}

const std::vector < std::string > &
QtRootController::
getNTupleNames ( const std::string & filename )
{
  PyApp::lock();

  try {
    const std::vector< std::string > & names 
      =  m_instance -> getNTupleNames ( filename );
    PyApp::unlock ();
    return names;
  }
  catch ( const std::runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  static const std::vector < std::string > names;
  // never reach following but VC++ insists on return value.
  return names;
}
