/* -*- mode: c++ -*- */

/** @file
 
DataRepController class interface

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataRepController.h,v 1.20 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _DataRepController_H_
#define _DataRepController_H_

#include "pattern/libhippo.h"


#include <string>
#include <vector>

namespace hippodraw {
  class DataRep;
  class DataSource;
  class NTuple;
  class PlotterBase;

/** A singleton class that is the interface between GUI and the
    DataRep.  It frequently implements the application logic which
    should neither be in the GUI nor in the display objects.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API DataRepController
{

private:

  /** The pointer to the singleton object. */
  static DataRepController * s_instance;

  /** A empty vector of strings.  This vector is returned by member
      functions that can not obtain a reference from a DataRep. */
  std::vector< std::string > m_null_vector;

  /** A private copy constructor in order to avoid copying. */
  DataRepController ( const DataRepController & );

  /** Returns @c true if all the the string objects in the vector @a
      bindings are valid labels for the columns in DataSource @a tuple.
      Otherwise returns @c false. */
  bool checkBindings ( const DataSource * tuple, 
		       const std::vector < std::string > & bindings ) const;

protected:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  DataRepController();

public:

  /** Returns the pointer to the singleton instance. */
  static DataRepController * instance ();

  /** The destructor. */
  ~DataRepController();

  /** Returns the binding options, if any, for the named DataRep. If
      no binding options are available, return an empty vector. */
  const std::vector< std::string > & 
  bindingOptions ( const std::string & name );

  /** Returns the binding options, if any, for the DataRep. If
      no binding options are available, return an empty vector. */
  const std::vector< std::string > & 
  bindingOptions ( DataRep * rep );

  /** Returns the names of the types of DataRep objects available.
   */
  const std::vector < std::string > & names () const;

  /** Creates a new DataRep object of class @a name.  It should be a
      class that does not have NTuple bindings.  Returns the DataRep
      object if successful, otherwise a DataRepException is thrown.
  */
  DataRep * createDataRep ( const std::string & name );

  /** Creates a new DataRep object of class @a name. Connects
      the newly created DataRep to @a ntuple and sets the bindings to
      @a bindings.  Returns DataRep if successful, otherwise an
      DataRepException is thrown. 
  */
  DataRep * 
  createDataRep ( const std::string & name,
		  const DataSource * ntuple,
		  const std::vector< std::string > & bindings ) const;

  /** Sets the axis binding of the DataRep object.
   */
  void setAxisBindings ( DataRep * rep,
			 const DataSource * ntuple,
			 const std::vector< std::string > & bindings ) const;

  /** Changes the DataSource.  Attempt to change the DataSource for
      the DataRep object while preserving the bindings.  The object @a
      rep must be one that contains a NTupleProjector, otherwise an
      assertion is raised.  If the @a ntuple doesn't have column
      labels required, then the old data source is restored and a
      DataSourceException is thrown.
  */
  void changeDataSource ( DataRep * rep,
			  const DataSource * tuple );

  };

} // namespace hippodraw

#endif // _DataRepController_H_
