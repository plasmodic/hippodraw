/** @file

hippodraw::DataRepController class implementation

Copyright (C) 2002-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataRepController.cxx,v 1.39 2007/01/10 19:29:01 xiefang Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataRepController.h"

#include "datareps/DataRepException.h"
#include "datareps/DataRepFactory.h"
#include "datareps/FunctionRep.h"

#include "datasrcs/NTuple.h"
#include "datasrcs/TupleCut.h"

#include "plotters/PlotterBase.h"
#include "projectors/NTupleProjector.h"

#include <stdexcept>
#include <cassert>

using std::runtime_error;
using std::string;
using std::vector;

using namespace hippodraw;

DataRepController * DataRepController::s_instance = 0;

DataRepController::DataRepController ( )
{
}

DataRepController::~DataRepController ( )
{
  DataRepFactory * factory = DataRepFactory::instance ();
  delete factory;
}

DataRepController * DataRepController::instance ( )
{
  if ( s_instance == 0 ) {
    s_instance = new DataRepController ( );
  }
  return s_instance;
}

const vector< string > & 
DataRepController::bindingOptions ( const std::string & name )
{
  DataRepFactory * factory = DataRepFactory::instance ();
  DataRep * datarep = factory->prototype ( name );

  return bindingOptions ( datarep );
}

const vector < string > & 
DataRepController::bindingOptions ( DataRep * datarep )
{
  // Binding options only make sense for something related to a
  // NTupleProjector. If the downcast doesn't work, this function
  // return an empty vector to indicate that.
  ProjectorBase * pbase = datarep->getProjector ();
  NTupleProjector * ntProjector = dynamic_cast <NTupleProjector *> ( pbase );
  if ( ntProjector == 0 ) return m_null_vector;

  return ntProjector->bindingOptions ();
}

bool DataRepController::
checkBindings ( const DataSource * ntuple,
		const std::vector < std::string > & bindings ) const
{
  if ( ntuple == 0 ) return false;

  bool yes = true;
  vector < string > ::const_iterator first = bindings.begin ();
  while ( first != bindings.end() ) {
    const string & label = *first++;
    if ( label == "nil" ) continue;
    ntuple -> throwIfInvalidLabel ( label );
  }

  return yes;
}

const vector < string > &
DataRepController::
names () const
{
  DataRepFactory * factory = DataRepFactory::instance ();

  return factory -> names ();
}

DataRep *
DataRepController::
createDataRep ( const std::string & type )
{
  DataRep * rep = 0;
  DataRepFactory * factory = DataRepFactory::instance ();
  try {
    rep = factory -> create ( type ); // let exception be thrown
  }
  catch ( const FactoryException & e ) {
    const string what = e.what ();
    throw DataRepException ( what );
  }
  ProjectorBase * projector = rep -> getProjector ();
  projector -> addObserver ( rep );

  return rep;
}
				
DataRep * 
DataRepController::
createDataRep ( const std::string & type,
		const DataSource * tuple,
		const std::vector < std::string > & bindings ) const
{
  // Expansion needed by RootNTuple with multi-dimensional array.
  tuple->expandIfNeeded(bindings);

  bool ok = checkBindings ( tuple, bindings );
  if ( !ok ) return 0;

  DataRepFactory * factory = DataRepFactory::instance ();
  DataRep * rep = factory->create ( type ); // let the exception be thrown
  setAxisBindings ( rep, tuple, bindings );

  return rep;
}

void
DataRepController::
setAxisBindings ( DataRep * rep,
		  const DataSource * tuple,
		  const std::vector < std::string > & bindings ) const
{
  NTupleProjector * ntProjector =
    dynamic_cast < NTupleProjector * > ( rep->getProjector() );

  ntProjector->setNTuple ( tuple );
  DataSource * nt = const_cast < DataSource * > ( tuple );
  nt->addObserver ( ntProjector );
  rep->setAxisBindings ( bindings );

  const vector < const TupleCut * > & cuts = ntProjector -> getCutList ();
  unsigned int size = cuts.size ();
  for ( unsigned int i = 0; i < size; i++ ) {
    const TupleCut * tc = cuts[i];
    const string & label = tc -> getLabel ();
    unsigned int index = ntProjector -> indexOf ( label );
    TupleCut * cut = const_cast < TupleCut * > ( tc );
    cut -> setColumn ( index );
  }

  ntProjector->addObserver ( rep );
}

void
DataRepController::
changeDataSource ( DataRep * rep, const DataSource * tuple )
{
  ProjectorBase * pb = rep -> getProjector ();
  NTupleProjector * projector = dynamic_cast < NTupleProjector * > ( pb );
  assert ( projector != 0 );

  DataSource * source = projector -> getNTuple ();
  source -> removeObserver ( rep );

  const vector < string > & bindings = projector -> getAxisBindings ();

  try {
    setAxisBindings ( rep, tuple, bindings );
  }
  catch ( const runtime_error & e ) {
    setAxisBindings ( rep, source, bindings );
    throw e;
  }
}
