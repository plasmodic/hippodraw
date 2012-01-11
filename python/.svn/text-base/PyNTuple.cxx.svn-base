/** @file

hippodraw::PyNTuple class implementation.

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

<<<<<<< PyNTuple.cxx
$Id: PyNTuple.cxx,v 1.25 2006/11/27 21:52:06 pfkeb Exp $
=======
$Id: PyNTuple.cxx,v 1.25 2006/11/27 21:52:06 pfkeb Exp $
>>>>>>> 1.24

*/

#include "PyNTuple.h"

#include "PyApp.h"

#include <stdexcept>

using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;


PyNTuple::
PyNTuple ( const std::vector < std::string > & labels )
  : NTuple ( labels )
{
  // nothing else to be done.
}

PyNTuple::
PyNTuple ( unsigned int n )
  : NTuple ( n )
{
  // nothing else to be done
}

PyNTuple::
PyNTuple ( const PyNTuple & nt )
  : NTuple ( nt )
{
  // nothing else to be done
}

PyNTuple::
PyNTuple ( const NTuple & nt )
  : NTuple ( nt )
{
  // nothing else to be done
}

PyNTuple::
PyNTuple ( )
  : NTuple ( )
{
}

void
PyNTuple::
setTitle ( const std::string & title )
{
  PyApp::lock ();
  DataSource::setTitle ( title );
  PyApp::unlock ();
}

int
PyNTuple::
addColumn ( const std::string & label,
	    const std::vector < double > & column )
{
  PyApp::lock ();
  int index = -1;
  try {
    index = NTuple::addColumn ( label, column );
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();

  return index;
}

void
PyNTuple::
replaceColumn ( unsigned int i, const std::vector < double > & column )
{
  PyApp::lock ();
  try {
    NTuple::replaceColumn ( i, column );
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

void
PyNTuple::
replaceColumn ( const std::string & label,
		const std::vector < double > & column )
{
  PyApp::lock ();
  try {
    DataSource::replaceColumn ( label, column );
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

void
PyNTuple::
addRow ( const std::vector < double > & row )
{
  // at some point we needed this lock, but after to code was fixed so
  // PyCanvas checks for pending events this lock causes
  // run_test_scripts to hang when running with no pompt.

  PyApp::lock ();

  try { 
    NTuple::addRow ( row );
  }
  catch ( const runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }

  PyApp::unlock ();
}

void
PyNTuple::
clear ()
{
  PyApp::lock ();
  NTuple::clear ();
  PyApp::unlock ();
}

void
PyNTuple::
append ( const DataSource * source )
{
  PyApp::lock ();
  NTuple::append ( source );
  PyApp::unlock ();
}
