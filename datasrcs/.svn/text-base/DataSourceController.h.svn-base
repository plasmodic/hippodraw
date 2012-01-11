/* -*- mode: c++ -*- */

/** @file

DataSourceController class interface

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataSourceController.h,v 1.22 2006/09/28 23:24:27 pfkeb Exp $

*/

#ifndef _DataSourceController_H_
#define _DataSourceController_H_

#include "pattern/Observer.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#pragma warning(disable:4290) // advice from MSDN article and others
                              // VC++ is not following the standard
#endif

#include <stdexcept>
#include <vector>

namespace hippodraw {

class DataSource;
class Observable;

/** A singleton class that is the interface between GUI and the
    DataSource objects.  It implements the application logic which
    should neither be in the GUI nor in the DataSource objects.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API DataSourceController : private hippodraw::Observer
{

private:

  /** The pointer to the singleton object. */
  static DataSourceController * s_instance;

  /** A default constructor for avoiding creation except by itself. */
  DataSourceController();

  /** A private copy constructor in order to avoid copying. */
  DataSourceController ( const DataSourceController & );

  /** The type of STL container used to keep list of DataSource
    objects.
  */
  typedef std::vector < DataSource * > DataSourceList_t;

  /** The list of registered DataSource objects. 
   */
  DataSourceList_t m_sources;

  /** The list of files that were read.
   */
  DataSourceList_t m_ds_files;

  /** A temporary list of DataSource names. */
  mutable std:: vector < std::string > m_names;

  /** Base of name given to NTuple which is built in memory. */
  std::string m_base_name;

  /** Suffix added to base name given to NTuple which is built in
      memory. */
  int m_suffix;

  /** The current DataSource index.
   */
  int m_current_index;

public:

  /** Returns the pointer to the singleton instance. */
  static DataSourceController * instance ();

  /** Responds to the Observable having a change.  This implementation
      does noting.  The implementation is required because of the pure
      virtual function in the base class. 
  */
  virtual void update ( const Observable * );

  /** Responds to Observable being deleted.  If @a observer is an object
      derived from DataSource, then unregisters it. 
  */
  void willDelete ( const Observable * observee );

  /** Returns index of DataSource, or -1 if not found.

      @warning If more than one %DataSource has the same name, then
      the index of the first one found is returned.
   */
  int indexOfDataSource ( const std::string & name ) const;

  /** Returns the DataSource object registered as @a name.  If
      a object of the given name is found, returns a pointer to it,
      otherwise throws a std::runtime_error.

      @warning If more than one data source has the same name, the
      first one is returned.
  */
  DataSource * findDataSource ( const std::string & name ) 
    const throw ( std::runtime_error );

  /** Returns the DataSource object registered as @a name.  If
      the object is found, return a pointer to it.  If the object is
      not found, return a null pointer.

      @warning If more than one data source has the same name, then
      the first one found is returned.
  */
  DataSource * getDataSource ( const std::string & name ) const;

  /** Changes the name of the DataSource.  Changes the name of the
      registered %DataSource object designated by @a old_name, to the name
      @a new_name.
   */
  void changeName ( const std::string & old_name, 
		    const std::string & new_name );


  /** Clears and fills the vector with the registered DataSource
      objects.  If @a all is @c true, the default, returns vector of
      all DataSource objects registered with the controller. If @a all
      is @c false, then only returns the DataSource objects that were
      not created from a file.
   */
  void getDataSources ( std::vector < DataSource * > &,
			bool all = true ) const;

  /** Returns a vector of the current DataSource names.  @attention
   The order of the names should agree with the order from
   getDataSources.  This is so that the application can fill a GUI
   with names, and access the DataSource by index.
  */
  const std::vector < std::string > & getNTupleNames () const;

  /** Registers that the DataSource came from a file.
   */
  void registerDataSourceFile ( DataSource * ds );

  /** Register a DataSource.  A unique name will be generated for the
      NTuple.  
      @sa registerNTuple( const std::string &, NTuple * ).
   */
  std::string registerNTuple ( DataSource * nt );

  /** Register a DataSoruce.  Register a DataSource object @a source
      using the name @a key for later lookup.  DataSource objects need
      to be registered in order to be visible application wide using
      this singleton controller.
   */
  void registerNTuple ( const std::string &, DataSource * source );

  /** Un-registers the DataSource.  Un-registers the %DataSource from
      the list of available %DataSource objects.  If the object is not
      found, does nothing.
  */
  void unregisterNTuple ( const DataSource * nt );

  /** Returns @c true if the DataSource object was created from a file
      or was saved to a file, otherwise returns @c false.
   */
  bool isFromFile ( const DataSource * source ) const;

  /** Returns a pointer to the current DataSource.
   */
  DataSource * getCurrent () const;

  /** Sets the index to the current DataSource.
   */
  void setCurrentIndex ( int index );

};

} // namespace hippodraw

#endif // _DataSourceController_H_
